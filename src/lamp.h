#ifndef LAMP_h
#define LAMP_h

#include <Arduino.h>

#include "config.h"

class Lamp {
 public:
  Lamp(float _brightness[LAMP_LED_BRIGHNESS_COUNT]);

  void begin();
  void update();
  void toggle_state();

 private:
  uint8_t current_state;
  float brightness[LAMP_LED_BRIGHNESS_COUNT];
};

#endif