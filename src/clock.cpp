#include "clock.h"

#include <Arduino.h>
#include <RTClib.h>

#include "types.h"

Clock::Clock() : ready_to_start(false), rtc(), additionals{0, 0, 0} {}

void Clock::begin() { ready_to_start = rtc.begin(); }

void Clock::update(TIME_t* time) {
  DateTime now = rtc.now();

  time->second = now.second();
  time->minute = now.minute();
  time->hour = now.hour();
}

void Clock::increment_minute() { additionals.minute += 1; }

void Clock::increment_hour() { additionals.hour += 1; }

void Clock::set_time() {
  DateTime now = rtc.now();

  DateTime new_date = DateTime(now.year(), now.month(), now.day(),
                               (now.hour() + additionals.hour) % 24,
                               (now.minute() + additionals.minute) % 60,
                               (now.second() + additionals.second) % 60);

  rtc.adjust(new_date);
}

/**
 * @brief Must be a cloned time to be pass here
 *
 * @param time TIME_t
 */
void Clock::temporary_clock(TIME_t* time) {
  DateTime now = rtc.now();

  time->second = (now.second() + additionals.second) % 60;
  time->minute = (now.minute() + additionals.minute) % 60;
  time->hour = (now.hour() + additionals.hour) % 24;
}

void Clock::clear_additionals() {
  additionals.second = 0;
  additionals.minute = 0;
  additionals.hour = 0;
}
