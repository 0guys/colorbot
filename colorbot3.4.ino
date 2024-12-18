#include <Arduino.h>
#include "MotorControl.h"
#include "ColorSensor.h"
#include "UltrasonicSensor.h"
#include "PinConfig.h"
#include <ESP32Servo.h>
#include <Bluepad32.h>

// 伺服馬達對象
Servo xServo;

// 全局變數用於非阻塞式伺服馬達控制
bool servoIsMoving = false;
unsigned long servoStartTime = 0;
unsigned long servoDuration = 1000; // 1秒

bool autoMode = false;
bool lastButtonState = false;

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// 控制轉向狀態
bool isTurning = false;
unsigned long turnStartTime = 0;
unsigned long turnDuration = 500; // 0.5秒

// 控制移動狀態
bool isMoving = false;
unsigned long moveStartTime = 0;
unsigned long moveDuration = 1000; // 1秒

// 定義閾值和死區，重新命名以避免衝突
const int MY_AXIS_THRESHOLD = 100;  // 增加閾值
const int MY_DEADZONE = 50;         // 設定死區

// 定義滑動平均窗口大小
const int SMOOTHING_WINDOW = 5;

// 儲存歷史數據
int axisXHistory[SMOOTHING_WINDOW] = {0};
int axisYHistory[SMOOTHING_WINDOW] = {0};
int currentIndex = 0;

// 平滑函數
int smoothAxis(int* history, int newValue) {
  history[currentIndex] = newValue;
  int sum = 0;
  for (int i = 0; i < SMOOTHING_WINDOW; i++) {
    sum += history[i];
  }
  return sum / SMOOTHING_WINDOW;
}

// 函式宣告
void onConnectedController(ControllerPtr ctl);
void onDisconnectedController(ControllerPtr ctl);
void processControllers();
void processGamepad(ControllerPtr ctl);
void dumpGamepad(ControllerPtr ctl);

void setup() {
  // 初始化序列監視器
  Serial.begin(115200);
  Serial.println("Initializing System...");
  
  // 初始化馬達控制腳位
  initMotors();
  Serial.println("Motors Initialized");
  
  // 初始化顏色感測器
  initColorSensor();
  Serial.println("Color Sensor Initialized");
  
  // 初始化超音波感測器
  initUltrasonicSensors();
  Serial.println("Ultrasonic Sensors Initialized");
  
  // 初始化伺服馬達
  xServo.attach(xServoPin);
  xServo.write(90);  // 設置初始位置
  Serial.println("Servo Motor Initialized");
  
  // 初始化 Bluepad32
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
  BP32.enableVirtualDevice(false);
  Serial.println("Bluepad32 Initialized");
  
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
    Serial.printf("Color - R: %lu, G: %lu, B: %lu\n", colorVal.red, colorVal.green, colorVal.blue);
    
    // 檢查是否正在轉向
    if (isTurning) {
      if (millis() - turnStartTime >= turnDuration) {
        isTurning = false;
        stopAllMotors();
        Serial.println("Turn completed");
      }
    }
    
    // 檢查是否正在移動
    if (isMoving) {
      if (millis() - moveStartTime >= moveDuration) {
        isMoving = false;
        stopAllMotors();
        Serial.println("Move completed");
      }
    }
    
    // 如果沒有正在執行轉向或移動
    if (!isTurning && !isMoving) {
      // 避障條件：左右兩側距離都大於380公分
      if (distLeft > 380.0 && distRight > 380.0) {
        Serial.println("Both sides are clear - Turning Right");
        turnRight();
        isTurning = true;
        turnStartTime = millis();
      }
      // 前方障礙物檢測
      else if (distFront < 10.0) {  // 前方障礙物
        if (distRight > distLeft) {
          Serial.println("Obstacle Ahead - Turning Right");
          turnRight();
          isTurning = true;
          turnStartTime = millis();
        } else {
          Serial.println("Obstacle Ahead - Turning Left");
          turnLeft();
          isTurning = true;
          turnStartTime = millis();
        }
      }
      
      // 顏色識別邏輯
      if (colorVal.red < colorVal.blue && colorVal.red < colorVal.green && colorVal.red < 10) {
        Serial.println("Red Color Detected - Moving Backward");
        moveBackward();
        isMoving = true;
        moveStartTime = millis();
      } else if (colorVal.blue < colorVal.red && colorVal.blue < colorVal.green) {
        Serial.println("Blue Color Detected - Moving Forward");
        moveForward();
        isMoving = true;
        moveStartTime = millis();
      } else if (colorVal.green < colorVal.red && colorVal.green < colorVal.blue) {
        Serial.println("Green Color Detected - Rotate Servo");
        if (!servoIsMoving) {
          servoIsMoving = true;
          servoStartTime = millis();
          xServo.write(180);
        }
      }
    }
    
    // 處理伺服馬達的非阻塞旋轉
    if (servoIsMoving) {
      if (millis() - servoStartTime >= servoDuration) {
        xServo.write(0);
        servoIsMoving = false;
        Serial.println("Servo rotation completed");
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
  // 判斷 PS4 三角形按鈕是否被按下
  bool currentButtonState = (ctl->buttons() == 0x0088);

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

    int rawAxisX = ctl->axisX();
    int rawAxisY = ctl->axisY();
    int rawAxisRY = ctl->axisRY();

    // 平滑處理
    int smoothAxisX = smoothAxis(axisXHistory, rawAxisX);
    int smoothAxisY = smoothAxis(axisYHistory, rawAxisY);

    // 處理死區
    if (abs(smoothAxisY) < MY_DEADZONE) {
      smoothAxisY = 0;
    }

    if (abs(smoothAxisX) < MY_DEADZONE) {
      smoothAxisX = 0;
    }

    // 列印軸向值以進行除錯
    Serial.printf("Raw AxisX: %d, Raw AxisY: %d\n", rawAxisX, rawAxisY);
    Serial.printf("Smooth AxisX: %d, Smooth AxisY: %d\n", smoothAxisX, smoothAxisY);

    // 控制移動方向
    if (smoothAxisY <= -MY_AXIS_THRESHOLD) {  // 前進
      moveForward();    // 全速前進
    } else if (smoothAxisY >= MY_AXIS_THRESHOLD) {      // 後退
      moveBackward();   // 全速後退
    }

    if (smoothAxisX <= -MY_AXIS_THRESHOLD) {  // 左轉
      turnLeft();       // 全速左轉
    } else if (smoothAxisX >= MY_AXIS_THRESHOLD) {  // 右轉
      turnRight();      // 全速右轉
    }

    // 停止馬達
    if (smoothAxisY == 0 && smoothAxisX == 0) {
      stopAllMotors();
    }

    // 伺服馬達控制（添加閾值判斷）
    if (abs(rawAxisRY) > 25) {  // 伺服馬達閾值可以根據需要調整
      int servoPos = map(rawAxisRY, -512, 512, 0, 180);
      servoPos = constrain(servoPos, 0, 180);
      xServo.write(servoPos);
    }

    // 更新滑動平均索引
    currentIndex = (currentIndex + 1) % SMOOTHING_WINDOW;

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
