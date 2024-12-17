#include "MotorControl.h"
#include <Arduino.h>
#include "PinConfig.h"

// PWM 設定
const int PWM_FREQ = 1000; // 1kHz
const int PWM_RES = 8;
const int CHANNEL_ENA = 0;
const int CHANNEL_ENB = 1;
const int CHANNEL_ENA2 = 2;
const int CHANNEL_ENB2 = 3;

// 全局轉向變數
bool isTurning = false;
unsigned long turnStartTime = 0;
unsigned long turnDuration = 0; // 轉向持續時間（毫秒）

// 設定PWM
inline void setMotorPWM(int channel, int duty) {
  ledcWrite(channel, duty);
}

void initMotors() {
  pinMode(IN1pin, OUTPUT);
  pinMode(IN2pin, OUTPUT);
  pinMode(IN3pin, OUTPUT);
  pinMode(IN4pin, OUTPUT);
  pinMode(ENApin, OUTPUT);
  pinMode(ENBpin, OUTPUT);

  pinMode(IN1pin2, OUTPUT);
  pinMode(IN2pin2, OUTPUT);
  pinMode(IN3pin2, OUTPUT);
  pinMode(IN4pin2, OUTPUT);
  pinMode(ENApin2, OUTPUT);
  pinMode(ENBpin2, OUTPUT);

  ledcSetup(CHANNEL_ENA, PWM_FREQ, PWM_RES);
  ledcSetup(CHANNEL_ENB, PWM_FREQ, PWM_RES);
  ledcSetup(CHANNEL_ENA2, PWM_FREQ, PWM_RES);
  ledcSetup(CHANNEL_ENB2, PWM_FREQ, PWM_RES);

  ledcAttachPin(ENApin, CHANNEL_ENA);
  ledcAttachPin(ENBpin, CHANNEL_ENB);
  ledcAttachPin(ENApin2, CHANNEL_ENA2);
  ledcAttachPin(ENBpin2, CHANNEL_ENB2);

  stopAllMotors();
}

void moveForward(int speed, int duration) {
  // 設定所有馬達為前進方向
  digitalWrite(IN1pin, HIGH);
  digitalWrite(IN2pin, LOW);
  digitalWrite(IN3pin, HIGH);
  digitalWrite(IN4pin, LOW);

  digitalWrite(IN1pin2, HIGH);
  digitalWrite(IN2pin2, LOW);
  digitalWrite(IN3pin2, HIGH);
  digitalWrite(IN4pin2, LOW);

  setMotorPWM(CHANNEL_ENA, speed);
  setMotorPWM(CHANNEL_ENB, speed);
  setMotorPWM(CHANNEL_ENA2, speed);
  setMotorPWM(CHANNEL_ENB2, speed);

  if (duration > 0) {
    delay(duration);
    stopAllMotors();
  }
}

void moveBackward(int speed, int duration) {
  // 設定所有馬達為後退方向
  digitalWrite(IN1pin, LOW);
  digitalWrite(IN2pin, HIGH);
  digitalWrite(IN3pin, LOW);
  digitalWrite(IN4pin, HIGH);

  digitalWrite(IN1pin2, LOW);
  digitalWrite(IN2pin2, HIGH);
  digitalWrite(IN3pin2, LOW);
  digitalWrite(IN4pin2, HIGH);

  setMotorPWM(CHANNEL_ENA, speed);
  setMotorPWM(CHANNEL_ENB, speed);
  setMotorPWM(CHANNEL_ENA2, speed);
  setMotorPWM(CHANNEL_ENB2, speed);

  if (duration > 0) {
    delay(duration);
    stopAllMotors();
  }
}

void turnLeft(int duration) {
  // 左側馬達後退，右側馬達前進
  digitalWrite(IN1pin, LOW);
  digitalWrite(IN2pin, HIGH);
  digitalWrite(IN3pin, LOW);
  digitalWrite(IN4pin, HIGH);

  digitalWrite(IN1pin2, HIGH);
  digitalWrite(IN2pin2, LOW);
  digitalWrite(IN3pin2, HIGH);
  digitalWrite(IN4pin2, LOW);

  setMotorPWM(CHANNEL_ENA, 200);
  setMotorPWM(CHANNEL_ENB, 200);
  setMotorPWM(CHANNEL_ENA2, 200);
  setMotorPWM(CHANNEL_ENB2, 200);

  delay(duration);
  stopAllMotors();
}

void turnRight(int duration) {
  // 左側馬達前進，右側馬達後退
  digitalWrite(IN1pin, HIGH);
  digitalWrite(IN2pin, LOW);
  digitalWrite(IN3pin, HIGH);
  digitalWrite(IN4pin, LOW);

  digitalWrite(IN1pin2, LOW);
  digitalWrite(IN2pin2, HIGH);
  digitalWrite(IN3pin2, LOW);
  digitalWrite(IN4pin2, HIGH);

  setMotorPWM(CHANNEL_ENA, 200);
  setMotorPWM(CHANNEL_ENB, 200);
  setMotorPWM(CHANNEL_ENA2, 200);
  setMotorPWM(CHANNEL_ENB2, 200);

  delay(duration);
  stopAllMotors();
}

void stopAllMotors() {
  setMotorPWM(CHANNEL_ENA, 0);
  setMotorPWM(CHANNEL_ENB, 0);
  setMotorPWM(CHANNEL_ENA2, 0);
  setMotorPWM(CHANNEL_ENB2, 0);

  digitalWrite(IN1pin, LOW);
  digitalWrite(IN2pin, LOW);
  digitalWrite(IN3pin, LOW);
  digitalWrite(IN4pin, LOW);

  digitalWrite(IN1pin2, LOW);
  digitalWrite(IN2pin2, LOW);
  digitalWrite(IN3pin2, LOW);
  digitalWrite(IN4pin2, LOW);
}

void turnLeftNonBlocking(unsigned long duration) {
  if (!isTurning) {
    // 開始左轉
    isTurning = true;
    turnStartTime = millis();
    turnDuration = duration;

    // 左側馬達後退，右側馬達前進
    digitalWrite(IN1pin, LOW);
    digitalWrite(IN2pin, HIGH);
    digitalWrite(IN3pin, LOW);
    digitalWrite(IN4pin, HIGH);

    digitalWrite(IN1pin2, HIGH);
    digitalWrite(IN2pin2, LOW);
    digitalWrite(IN3pin2, HIGH);
    digitalWrite(IN4pin2, LOW);

    setMotorPWM(CHANNEL_ENA, 200);
    setMotorPWM(CHANNEL_ENB, 200);
    setMotorPWM(CHANNEL_ENA2, 200);
    setMotorPWM(CHANNEL_ENB2, 200);

    Serial.println("Non-blocking Turn Left Initiated");
  }
}

void turnRightNonBlocking(unsigned long duration) {
  if (!isTurning) {
    // 開始右轉
    isTurning = true;
    turnStartTime = millis();
    turnDuration = duration;

    // 左側馬達前進，右側馬達後退
    digitalWrite(IN1pin, HIGH);
    digitalWrite(IN2pin, LOW);
    digitalWrite(IN3pin, HIGH);
    digitalWrite(IN4pin, LOW);

    digitalWrite(IN1pin2, LOW);
    digitalWrite(IN2pin2, HIGH);
    digitalWrite(IN3pin2, LOW);
    digitalWrite(IN4pin2, HIGH);

    setMotorPWM(CHANNEL_ENA, 200);
    setMotorPWM(CHANNEL_ENB, 200);
    setMotorPWM(CHANNEL_ENA2, 200);
    setMotorPWM(CHANNEL_ENB2, 200);

    Serial.println("Non-blocking Turn Right Initiated");
  }
}

void checkTurning() {
  if (isTurning) {
    unsigned long currentMillis = millis();
    if (currentMillis - turnStartTime >= turnDuration) {
      stopAllMotors();
      isTurning = false;
      Serial.println("Non-blocking Turn Completed");
    }
  }
}
