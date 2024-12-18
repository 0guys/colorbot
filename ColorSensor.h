#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

#include <Arduino.h>

// 顏色讀取結構
struct ColorReading {
  unsigned long red;
  unsigned long green;
  unsigned long blue;
};

// 顏色感測器函數
void initColorSensor();
ColorReading readColor();

#endif
