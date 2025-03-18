#include "alarm.h"

#include <Arduino.h>

#include "types.h"

Alarm::Alarm()
    : current_alarm{0, 0, false}, rang_count(0), next_rang(1), ringing(false) {}

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