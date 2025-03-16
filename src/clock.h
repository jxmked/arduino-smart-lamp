#ifndef CLOCK_h
#define CLOCK_h

#include <Arduino.h>
#include <RTClib.h>

#include "types.h"

class Clock {
 public:
  void begin();
  void update(TIME_t* time);

 private:
  RTC_DS1307 rtc;
};

#endif