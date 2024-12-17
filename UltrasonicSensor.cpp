#include "UltrasonicSensor.h"
#include <Arduino.h>
#include "PinConfig.h"  // 若腳位在此定義

void initUltrasonicSensors() {
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);

  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);

  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);
}

static float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000UL);
  float distance = duration * 0.0343 / 2.0;
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
