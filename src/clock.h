#ifndef CLOCK_h
#define CLOCK_h

#include <Arduino.h>
#include <RTClib.h>

#include "types.h"

class Clock {
 public:
  Clock();

  bool ready_to_start;

  void begin();
  void update(TIME_t* time);
  void increment_minute();
  void increment_hour();
  void set_time();
  void temporary_clock(TIME_t* time);
  void clear_additionals();

 private:
  RTC_DS1307 rtc;
  TIME_t additionals;
};

#endif