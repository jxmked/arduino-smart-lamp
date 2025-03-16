#include "clock.h"

#include <Arduino.h>
#include <RTClib.h>

#include "types.h"

Clock::Clock() : ready_to_start(false), rtc() {}

void Clock::begin() { ready_to_start = rtc.begin(); }

void Clock::update(TIME_t* time) {
  DateTime now = rtc.now();

  time->second = now.second();
  time->minute = now.minute();
  time->hour = now.hour();
}
