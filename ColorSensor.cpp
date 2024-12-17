#include "ColorSensor.h"
#include <Arduino.h>
#include "PinConfig.h"  // 若腳位在此定義

void initColorSensor() {
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(outPin, INPUT);

  // 設定 TCS3200 輸出頻率為最高(根據資料表可調整)
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);
}

ColorReading readColor() {
  ColorReading cr;

  // Red
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  cr.red = pulseIn(outPin, digitalRead(outPin) == HIGH ? LOW : HIGH);

  // Blue
  digitalWrite(s3, HIGH);
  cr.blue = pulseIn(outPin, digitalRead(outPin) == HIGH ? LOW : HIGH);

  // Green
  digitalWrite(s2, HIGH);
  cr.green = pulseIn(outPin, digitalRead(outPin) == HIGH ? LOW : HIGH);

  return cr;
}
