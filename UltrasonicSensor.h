#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>

void initUltrasonicSensors();
float getFrontDistance();
float getLeftDistance();
float getRightDistance();

#endif
