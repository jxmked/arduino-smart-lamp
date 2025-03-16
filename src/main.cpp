#include <Arduino.h>
#include <TM1637.h>

#include "TimeInterval.h"
#include "config.h"
#include "display/display.h"
#include "types.h"

TIME_t time = {0, 58, 9};

TimeInterval timer = TimeInterval(100, 0, true);

Display display;

void setup() { display.begin(); }

void loop() {
  if (timer.marked()) {
    time.second++;
  }

  if (time.second > 59) {
    time.minute++;
    time.second %= 60;
  }

  if (time.minute > 59) {
    time.hour = (time.hour + 1) % 24;
  }

  time.minute %= 60;

  display.display_time(time);
}
