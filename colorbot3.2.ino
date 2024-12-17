#include <Arduino.h>
#include "MotorControl.h"
#include "ColorSensor.h"
#include "UltrasonicSensor.h"
#include "PinConfig.h"
#include <ESP32Servo.h>
#include <Bluepad32.h>

Servo xServo;

bool autoMode = false;
bool lastButtonState = false;

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// 轉向控制（非阻塞式）
// 在 MotorControl.cpp 中已定義全局變數
extern bool isTurning;
extern unsigned long turnStartTime;
extern unsigned long turnDuration;

// 函式宣告
void onConnectedController(ControllerPtr ctl);
void onDisconnectedController(ControllerPtr ctl);
void processControllers();
void processGamepad(ControllerPtr ctl);
void dumpGamepad(ControllerPtr ctl);

// 伺服馬達腳位
extern const int xServoPin;  // 在 PinConfig.h 有定義

void setup() {
  Serial.begin(115200);

  // 初始化各模組
  initMotors();
  initColorSensor();
  initUltrasonicSensors();

  // 初始化伺服馬達
  xServo.attach(xServoPin);
  xServo.write(90);  // 設置初始位置

  // 初始化 Bluepad32
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
  BP32.enableVirtualDevice(false);

  Serial.println("System Initialized");
}

void loop() {
  // 更新 Bluepad32 狀態
  bool dataUpdated = BP32.update();
  if (dataUpdated) {
    processControllers();
  }

  // 自動模式
  if (autoMode) {
    // 讀取感測器數據
    ColorReading colorVal = readColor();
    float distFront = getFrontDistance();
    float distLeft = getLeftDistance();
    float distRight = getRightDistance();

    // 打印感測器數據（除錯用）
    Serial.printf("Distances - Front: %.2f cm, Left: %.2f cm, Right: %.2f cm\n", distFront, distLeft, distRight);
    Serial.printf("Color - R: %d, G: %d, B: %d\n", colorVal.red, colorVal.green, colorVal.blue);

    // 檢查是否正在轉向
    checkTurning();

    // 如果正在轉向，暫停其他動作
    if (!isTurning) {
      // 新增避障條件：左右兩側距離都大於380公分
      if (distLeft > 380.0 && distRight > 380.0) {
        Serial.println("Both sides are clear - Turning Right");
        turnRightNonBlocking(500);  // 持續500毫秒
      }
      // 前方障礙物檢測
      else if (distFront < 10.0) {  // 前方障礙物
        if (distRight > distLeft) {
          Serial.println("Obstacle Ahead - Turning Right");
          turnRightNonBlocking(500);
        } else {
          Serial.println("Obstacle Ahead - Turning Left");
          turnLeftNonBlocking(500);
        }
      }

      // 顏色識別邏輯
      if (colorVal.red < colorVal.blue && colorVal.red < colorVal.green && colorVal.red < 10) {
        Serial.println("Red Color Detected - Moving Backward");
        moveBackward(200, 1000);  // 速度200，持續1秒
      } else if (colorVal.blue < colorVal.red && colorVal.blue < colorVal.green) {
        Serial.println("Blue Color Detected - Moving Forward");
        moveForward(200, 1000);  // 速度200，持續1秒
      } else if (colorVal.green < colorVal.red && colorVal.green < colorVal.blue) {
        Serial.println("Green Color Detected - Rotate Servo");
        xServo.write(180);
        delay(1000);  // 持續1秒
        xServo.write(0);
      }
    }

    // 短暫延遲
    delay(50);
  }

  // 添加小延遲以避免過度佔用 CPU
  delay(10);
}

// 控制器相關函式實作

void processGamepad(ControllerPtr ctl) {
  // 判斷特定按鍵（如 SHARE）是否被按下
  // 注意：Bluepad32 庫可能使用不同的按鍵定義，請根據實際情況調整
  // 例如，使用 isKeyPressed() 和對應的按鍵常數
  bool currentButtonState = (ctl->buttons() == 0x0008);  // 根據 Bluepad32 定義調整

  // 按鍵狀態變化
  if (currentButtonState && !lastButtonState) {
    autoMode = !autoMode;

    if (autoMode) {
      ctl->setColorLED(0, 0, 255);  // 設置 LED 為藍色
      Serial.println("Auto Mode Activated");
    } else {
      ctl->setColorLED(0, 255, 0);  // 設置 LED 為綠色
      Serial.println("Manual Mode Activated");
    }
  }

  lastButtonState = currentButtonState;

  // 手動模式控制
  if (!autoMode) {
    ctl->setColorLED(0, 255, 0);  // 設置 LED 為綠色

    int axisX = ctl->axisX();
    int axisY = ctl->axisY();
    int axisRY = ctl->axisRY();

    // 控制移動方向
    if (axisY <= -25) {  // 前進
      int motorSpeed = map(axisY, -512, -25, 255, 70);
      moveForward(motorSpeed, 0);  // 持續前進
    } else if (axisY >= 25) {      // 後退
      int motorSpeed = map(axisY, 25, 512, 70, 255);
      moveBackward(motorSpeed, 0);  // 持續後退
    }

    if (axisX <= -25) {  // 左轉
      int motorSpeed = map(axisX, -512, -25, 255, 70);
      turnLeftNonBlocking(0);  // 持續左轉（非阻塞式）
    } else if (axisX >= 25) {  // 右轉
      int motorSpeed = map(axisX, 25, 512, 70, 255);
      turnRightNonBlocking(0);  // 持續右轉（非阻塞式）
    }

    // 停止馬達
    if (axisY > -25 && axisY < 25 && axisX > -25 && axisX < 25) {
      stopAllMotors();
    }

    // 伺服馬達控制（添加閾值判斷）
    if (abs(axisRY) > 25) {
      int servoPos = map(axisRY, -512, 512, 0, 180);
      servoPos = constrain(servoPos, 0, 180);
      xServo.write(servoPos);
    }

    // 打印控制器狀態（除錯用）
    dumpGamepad(ctl);
  }
}

void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller connected, index=%d\n", i);
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n",
                    ctl->getModelName().c_str(),
                    properties.vendor_id,
                    properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot = true;
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println("CALLBACK: Controller connected, but no empty slot");
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      foundController = true;
      break;
    }
  }

  if (!foundController) {
    Serial.println("CALLBACK: Controller disconnected, not found in myControllers");
  }
}

void processControllers() {
  for (auto myController : myControllers) {
    if (myController && myController->isConnected() && myController->hasData()) {
      if (myController->isGamepad()) {
        processGamepad(myController);
      } else {
        Serial.println("Unsupported controller");
      }
    }
  }
}

void dumpGamepad(ControllerPtr ctl) {
  Serial.printf(
    "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
    "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
    ctl->index(),
    ctl->dpad(),
    ctl->buttons(),
    ctl->axisX(),
    ctl->axisY(),
    ctl->axisRX(),
    ctl->axisRY(),
    ctl->brake(),
    ctl->throttle(),
    ctl->miscButtons(),
    ctl->gyroX(),
    ctl->gyroY(),
    ctl->gyroZ(),
    ctl->accelX(),
    ctl->accelY(),
    ctl->accelZ());
}
