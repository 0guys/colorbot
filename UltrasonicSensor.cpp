#include "UltrasonicSensor.h"
#include "PinConfig.h"

// 初始化超音波感測器
void initUltrasonicSensors() {
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);
  
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);
}

// 計算距離
static float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000UL); // 30ms 超時
  if (duration == 0) {
    return 999.0; // 超時
  }
  
  float distance = duration * 0.0343 / 2.0; // cm
  return distance;
}

float getFrontDistance() {
  return getDistance(trigPinFront, echoPinFront);
}

float getLeftDistance() {
  return getDistance(trigPinLeft, echoPinLeft);
}

float getRightDistance() {
  return getDistance(trigPinRight, echoPinRight);
}
