#include "alarm.h"

#include <Arduino.h>

#include "types.h"

Alarm::Alarm()
    : current_alarm{0, 0, false},
      rang_count(0),
      next_rang(1),
      ringing(false),
      adjustments{0, 0, false} {}

void Alarm::begin() {}
void Alarm::load_data(ALARM_EVENT_t data) { current_alarm = data; }

bool Alarm::is_due(TIME_t time) {
  if (!current_alarm.enabled) return false;
  if (rang_count == next_rang) return false;

  if (time.minute >= current_alarm.minute) {
    if (time.hour == current_alarm.hour) {
      ringing = true;
      rang_count++;
    }
  }

  return ringing;
}

void Alarm::activate() { rang_count = (next_rang - 1); }

void Alarm::deactivate() {
  ringing = false;
  rang_count = 0;
}

void Alarm::snooze() {
  ringing = false;
  next_rang++;
}

bool Alarm::is_ringing() { return ringing; }

void Alarm::increase_minute() { adjustments.minute++; }

void Alarm::increase_hour() { adjustments.hour++; }

ALARM_EVENT_t Alarm::get_alarm_data() {
  uint8_t r_minute =
      (uint8_t)((current_alarm.minute + adjustments.minute) % 60);
  uint8_t r_hour = (uint8_t)((current_alarm.hour + adjustments.hour) % 24);

  return {r_minute, r_hour, true};
}

void Alarm::clear_adjustments() { adjustments = {0, 0, false}; }