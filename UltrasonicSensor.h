#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>

// 超音波感測器函數
void initUltrasonicSensors();
float getFrontDistance();
float getLeftDistance();
float getRightDistance();

#endif


