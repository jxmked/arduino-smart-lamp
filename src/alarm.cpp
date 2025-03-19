#include "alarm.h"

#include <Arduino.h>

#include "types.h"

Alarm::Alarm()
    : current_alarm{0, 0, false}, ringing(false), adjustments{0, 0, false} {}

void Alarm::begin() {}
void Alarm::load_data(ALARM_EVENT_t data) { current_alarm = data; }

bool Alarm::is_due() {
  if (!current_alarm.enabled) return false;

  if (day == time.day)
    if (time.minute >= current_alarm.minute)
      if (time.hour == current_alarm.hour) ringing = true;

  return ringing;
}

void Alarm::activate() {
  // Check if the alarm has over due
  if (time.hour >= current_alarm.hour) {
    if (time.minute >= current_alarm.minute) {
      // if over due, set the day when the alarm
      // will trigger. Must be like tomorrow
      day = time.day + 1;
      return;
    }
  }

  day = time.day;
}

void Alarm::deactivate() {
  ringing = false;
  current_alarm.enabled = false;
}

void Alarm::snooze() {
  day = day + 1;
  ringing = false;
}

bool Alarm::is_ringing() { return ringing; }

void Alarm::increase_minute() { adjustments.minute++; }

void Alarm::increase_hour() { adjustments.hour++; }

ALARM_EVENT_t Alarm::get_alarm_data() {
  uint8_t r_minute =
      (uint8_t)((current_alarm.minute + adjustments.minute) % 60);
  uint8_t r_hour = (uint8_t)((current_alarm.hour + adjustments.hour) % 24);

  return {r_hour, r_minute, true};
}

void Alarm::clear_adjustments() { adjustments = {0, 0, false}; }

void Alarm::set_day(uint8_t _day) { day = _day; }

void Alarm::update(TIME_t _time) { time = _time; }