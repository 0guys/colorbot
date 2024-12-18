#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

// 馬達控制函數
void initMotors();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopAllMotors();

#endif
