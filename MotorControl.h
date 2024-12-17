#ifndef MOTOR_CONTROL_H 
#define MOTOR_CONTROL_H

#include <Arduino.h>

// 馬達控制函數
void initMotors();
void moveForward(int speed = 200, int duration = 3000);
void moveBackward(int speed = 200, int duration = 3000);
void turnLeft(int duration = 500);
void turnRight(int duration = 500);
void stopAllMotors();

// 非阻塞式轉向函數
void turnLeftNonBlocking(unsigned long duration);
void turnRightNonBlocking(unsigned long duration);
void checkTurning();

#endif

