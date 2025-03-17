#include "display.h"

#include <Arduino.h>

#include "config.h"
#include "types.h"

Display::Display() : sevseg() {}

void Display::begin() {
  byte numDigits = 4;
  byte digitPins[4] = {SEG_D_1, SEG_D_2, SEG_D_3, SEG_D_4};
  byte segmentPins[7] = {
      SEG_L_S0, SEG_L_S1, SEG_L_S2, SEG_L_S3, SEG_L_S4, SEG_L_S5, SEG_L_S6,
  };

  bool resistorsOnSegments = false;
  byte hardwareConfig = COMMON_ANODE;
  bool updateWithDelays = false;
  bool leadingZeros = false;
  bool disableDecPoint = true;

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins,
               resistorsOnSegments, updateWithDelays, leadingZeros,
               disableDecPoint);
  sevseg.setBrightness(100);
}

void Display::display_time(TIME_t time) {
  uint8_t minute = time.minute;
  uint8_t hour = time.hour;

  // We move the hour to the tenth to make room for minute
  // at the last 2 digit
  uint16_t to_display = (hour * 100) + minute;

  sevseg.setNumber(to_display);

  sevseg.refreshDisplay();
}

void Display::display_err(bool with_refresh) {
  sevseg.setChars("Err");

  if (with_refresh) sevseg.refreshDisplay();
}
