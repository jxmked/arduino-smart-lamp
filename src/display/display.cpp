#ifndef DISPLAY_h
#define DISPLAY_h

#include "display.h"

#include <TM1637.h>

#include "config.h"

TM1637 seg(SEVEN_SEGMENT_CLK_PIN, SEVEN_SEGMENT_DIO_PIN);

Display::Display() {}

void Display::begin() {
  seg.init();
  seg.begin();
  seg.setBrightnessPercent(100);
}

void Display::display() {}

#endif