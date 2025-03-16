#ifndef DISPLAY_h
#define DISPLAY_h

#include "display.h"

#include <TM1637.h>

#include "config.h"
#include "types.h"

TM1637 seg(SEVEN_SEGMENT_CLK_PIN, SEVEN_SEGMENT_DIO_PIN);

Display::Display() : colon(true) {}

void Display::begin() {
  seg.init();
  seg.begin();
  seg.setBrightnessPercent(100);
}

void Display::display(TIME_t time) {
  uint8_t second = time.second;
  uint8_t minute = time.minute;

  
}

#endif