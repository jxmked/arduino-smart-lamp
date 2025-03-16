#include "display.h"

#include <Arduino.h>
#include <TM1637.h>

#include "config.h"
#include "types.h"

Display::Display()
    : colon(ColonState::ACTIVE),
      tm(SEVEN_SEGMENT_CLK_PIN, SEVEN_SEGMENT_DIO_PIN) {}

void Display::begin() {
  tm.init();
  tm.begin();
  tm.setBrightnessPercent(100);
}

void Display::display_time(TIME_t time) {
  uint8_t second = time.second;
  uint8_t minute = time.minute;
  uint8_t hour = time.hour;

  switch (colon) {
    case ColonState::KEEP_ON:
      tm.colonOn();
      break;

    case ColonState::ACTIVE:
      if (time.second % 2 == 0) {
        tm.colonOff();
      } else {
        tm.colonOn();
      }
      break;

    default:
      tm.colonOff();
      break;
  }

  if (hour < 10) {
    tm.display((hour * 100) + minute, false, false, 1);
  } else {
    tm.display((hour * 100) + minute, false, false, 0);
  }
}

void Display::display_err() { tm.display("Err", false, false, 1); }
