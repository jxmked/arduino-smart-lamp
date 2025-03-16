#include "display.h"

#include <Arduino.h>
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

  switch (colon) {
    case ColonState::KEEP_ON:
      tm.colonOn();
      break;

    case ColonState::ACTIVE:
      tm.switchColon();
      break;

    default:
      tm.colonOff();
      break;
  }

  // tm.clearScreen();

  if (hour < 10) {
    tm.display((hour * 100) + minute, false, false, 1);
  } else {
    tm.display((hour * 100) + minute, false, false, 0);
  }

  // delete[] time;
}

void Display::display_err() {
  // tm.clearScreen();
  tm.display("Err", false, false, 1);
}
