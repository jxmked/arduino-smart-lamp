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

void Display::display_time(TIME_t time, BLINKING_SET blinking) {
  sevseg.refreshDisplay();

  uint8_t minute = time.minute;
  uint8_t hour = time.hour;

  // We move the hour to the tenth to make room for minute
  // at the last 2 digit
  uint16_t to_display = (hour * 100) + minute;

  sevseg.setNumber(to_display);

  switch (blinking) {
    case BLINKING_SET::ALL:
      sevseg.blank();
      break;

    case BLINKING_SET::SET_A:
      sevseg.setSegmentsDigit(0, 0x0);
      sevseg.setSegmentsDigit(1, 0x0);
      break;

    case BLINKING_SET::SET_B:
      sevseg.setSegmentsDigit(2, 0x0);
      sevseg.setSegmentsDigit(3, 0x0);
      break;

    case BLINKING_SET::NONE:
      // What should I do????
      break;

    default:
      break;
  }
}

void Display::display_err(bool with_refresh) {
  sevseg.setChars("Err");

  sevseg.refreshDisplay();
}

void Display::display_alarm_on() {
  sevseg.setSegmentsDigit(0, 0x3F);
  sevseg.setSegmentsDigit(0, 0x54);

  sevseg.refreshDisplay();
}

void Display::display_alarm_off() {
  sevseg.setSegmentsDigit(0, 0x3F);
  sevseg.setSegmentsDigit(0, 0x71);
  sevseg.setSegmentsDigit(0, 0x71);

  sevseg.refreshDisplay();
}

void Display::blank() {
  sevseg.blank();
  sevseg.refreshDisplay();
}