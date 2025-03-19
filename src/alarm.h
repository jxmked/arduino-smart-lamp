#ifndef ALARM_h
#define ALARM_h

#include <Arduino.h>

#include "types.h"

class Alarm {
 public:
  Alarm();
  void begin();
  void load_data(ALARM_EVENT_t data);
  bool is_due();
  void activate();
  void deactivate();
  void snooze();
  bool is_ringing();
  void increase_minute();
  void increase_hour();
  ALARM_EVENT_t get_alarm_data();
  void clear_adjustments();
  void set_day(uint8_t _day);
  void update(TIME_t _time);

 private:
  ALARM_EVENT_t current_alarm;
  TIME_t time;
  bool ringing;
  ALARM_EVENT_t adjustments;
  uint8_t day;
};

#endif