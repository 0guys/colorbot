#ifndef PIN_CONFIG_H  
#define PIN_CONFIG_H

// 馬達控制腳位定義（全速運作，無 PWM 控制）
const int IN1pin = 13;    // 左前馬達控制
const int IN2pin = 14;    // 左前馬達控制
const int IN3pin = 15;    // 右前馬達控制
const int IN4pin = 16;    // 右前馬達控制

const int IN1pin2 = 17;    // 左後馬達控制（重新分配至 GPIO 17）
const int IN2pin2 = 18;    // 左後馬達控制
const int IN3pin2 = 19;    // 右後馬達控制
const int IN4pin2 = 21;    // 右後馬達控制

// 顏色感測器 (TCS3200)
const int s0 = 22;
const int s1 = 23;
const int s2 = 25;
const int s3 = 26;
const int outPin = 20;     // 重新分配至 GPIO 20，避免與馬達控制腳位衝突

// 超音波感測器
const int trigPinFront = 32;
const int echoPinFront = 34;

const int trigPinLeft = 33;
const int echoPinLeft = 35;

const int trigPinRight = 4;
const int echoPinRight = 36; // 改為 GPIO 36，避免使用 GPIO 0

// 伺服馬達
const int xServoPin = 2;

#endif
