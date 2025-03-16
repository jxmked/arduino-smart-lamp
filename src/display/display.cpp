#ifndef DISPLAY_h
#define DISPLAY_h

#include "display.h"

#include <TM1637.h>

#include "config.h"
#include "types.h"

TM1637 tm(SEVEN_SEGMENT_CLK_PIN, SEVEN_SEGMENT_DIO_PIN);

Display::Display() : colon(ColonState::ACTIVE) {}

void Display::begin() {
  tm.init();
  tm.begin();
  tm.setBrightnessPercent(100);
}

void Display::display_time(TIME_t time) {
  uint8_t second = time.second;
  uint8_t minute = time.minute;
  uint8_t hour = time.hour;

  if (colon == ColonState::KEEP_ON ||
      (colon == ColonState::ACTIVE && second % 2 == 1)) {
    tm.colonOn();
  } else {
    tm.colonOff();
  }

  tm.display(hour, false, true, 0);
  tm.display(minute, false, true, 3);
}

#endif