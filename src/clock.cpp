#include "clock.h"

#include <Arduino.h>
#include <RTClib.h>

#include "types.h"

Clock::Clock() : rtc() {}

void Clock::begin() {}

void Clock::update(TIME_t* time) {}