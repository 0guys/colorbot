#include "ColorSensor.h"
#include "PinConfig.h"

// 初始化顏色感測器
void initColorSensor() {
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(outPin, INPUT);
  
  // 設定 TCS3200 輸出頻率為最高
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);
}

// 讀取顏色
ColorReading readColor() {
  ColorReading cr;
  
  // 測量紅色
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  delay(100); // 等待感測器穩定
  cr.red = pulseIn(outPin, HIGH, 1000000); // 1秒超時
  
  // 測量藍色
  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  delay(100); // 等待感測器穩定
  cr.blue = pulseIn(outPin, HIGH, 1000000); // 1秒超時
  
  // 測量綠色
  digitalWrite(s2, HIGH);
  digitalWrite(s3, LOW);
  delay(100); // 等待感測器穩定
  cr.green = pulseIn(outPin, HIGH, 1000000); // 1秒超時
  
  // 處理超時情況
  if (cr.red == 0) cr.red = 999;
  if (cr.blue == 0) cr.blue = 999;
  if (cr.green == 0) cr.green = 999;
  
  return cr;
}

