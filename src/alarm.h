#ifndef ALARM_h
#define ALARM_h

#include <Arduino.h>

#include "types.h"

class Alarm {
 public:
  Alarm();
  void begin();
  void load_data(ALARM_EVENT_t data);
  bool is_due(TIME_t time);
  void activate();
  void deactivate();
  void snooze();
  bool is_ringing();

 private:
  ALARM_EVENT_t current_alarm;
  uint8_t rang_count;
  uint8_t next_rang;
  bool ringing;
};

#endif