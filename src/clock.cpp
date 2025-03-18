#include "clock.h"

#include <Arduino.h>
#include <RTClib.h>

#include "types.h"

#define ALARM_NVRAM_ALARM_ENABLED 0x01
#define ALARM_NVRAM_MINUTE 0x2
#define ALARM_NVRAM_HOUR 0x3
#define ALARM_NVRAM_ALARM_SET 0x4

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

ALARM_EVENT_t Clock::get_alarm_data() {
  ALARM_EVENT_t data = {
      rtc.readnvram(ALARM_NVRAM_MINUTE) % 60,
      rtc.readnvram(ALARM_NVRAM_HOUR) % 24,
      rtc.readnvram(ALARM_NVRAM_ALARM_ENABLED) ? true : false};

  return data;
}

void Clock::set_alarm_data(ALARM_EVENT_t data) {
  rtc.writenvram(ALARM_NVRAM_MINUTE, data.minute);
  rtc.writenvram(ALARM_NVRAM_HOUR, data.hour);
  rtc.writenvram(ALARM_NVRAM_ALARM_ENABLED, data.enabled);
}

bool Clock::alarm_is_set() {
  return rtc.readnvram(ALARM_NVRAM_ALARM_SET) ? true : false;
}

bool Clock::alarm_is_set(bool state) {
  rtc.writenvram(ALARM_NVRAM_ALARM_SET, state);

  return alarm_is_set();
}