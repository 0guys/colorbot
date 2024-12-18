#include "MotorControl.h"
#include "PinConfig.h"

// 初始化馬達控制腳位為輸出
void initMotors() {
  pinMode(IN1pin, OUTPUT);
  pinMode(IN2pin, OUTPUT);
  pinMode(IN3pin, OUTPUT);
  pinMode(IN4pin, OUTPUT);
  
  pinMode(IN1pin2, OUTPUT);
  pinMode(IN2pin2, OUTPUT);
  pinMode(IN3pin2, OUTPUT);
  pinMode(IN4pin2, OUTPUT);
  
  // 初始狀態為停止
  stopAllMotors();
}

// 前進
void moveForward() {
  digitalWrite(IN1pin, LOW);  
  digitalWrite(IN2pin, HIGH);
  
  digitalWrite(IN3pin, LOW); 
  digitalWrite(IN4pin, HIGH);
  
  digitalWrite(IN1pin2, LOW); 
  digitalWrite(IN2pin2, HIGH);
  
  digitalWrite(IN3pin2, HIGH);
  digitalWrite(IN4pin2, LOW);
}


// 後退
void moveBackward() {
  digitalWrite(IN1pin, HIGH);
  digitalWrite(IN2pin, LOW);
  
  digitalWrite(IN3pin, HIGH);
  digitalWrite(IN4pin, LOW);
  
  digitalWrite(IN1pin2, HIGH);
  digitalWrite(IN2pin2, LOW);
  
  digitalWrite(IN3pin2, LOW);
  digitalWrite(IN4pin2, HIGH);
}

// 左轉
void turnLeft() {
  digitalWrite(IN1pin, LOW);
  digitalWrite(IN2pin, HIGH);
  
  digitalWrite(IN3pin, HIGH);
  digitalWrite(IN4pin, LOW);
  
  digitalWrite(IN1pin2, LOW);
  digitalWrite(IN2pin2, HIGH);
  
  digitalWrite(IN3pin2, LOW);
  digitalWrite(IN4pin2, HIGH);
}

// 右轉
void turnRight() {
  digitalWrite(IN1pin, HIGH);
  digitalWrite(IN2pin, LOW);
  
  digitalWrite(IN3pin, LOW);
  digitalWrite(IN4pin, HIGH);
  
  digitalWrite(IN1pin2, HIGH);
  digitalWrite(IN2pin2, LOW);
  
  digitalWrite(IN3pin2, HIGH);
  digitalWrite(IN4pin2, LOW);
}

// 停止所有馬達
void stopAllMotors() {
  digitalWrite(IN1pin, LOW);
  digitalWrite(IN2pin, LOW);
  
  digitalWrite(IN3pin, LOW);
  digitalWrite(IN4pin, LOW);
  
  digitalWrite(IN1pin2, LOW);
  digitalWrite(IN2pin2, LOW);
  
  digitalWrite(IN3pin2, LOW);
  digitalWrite(IN4pin2, LOW);
}
