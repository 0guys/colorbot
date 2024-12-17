#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

#include <Arduino.h>

struct ColorReading {
  int red;
  int green;
  int blue;
};

void initColorSensor();
ColorReading readColor();

#endif
