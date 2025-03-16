#include <Arduino.h>
#include <TM1637.h>

#include "TimeInterval.h"
#include "clock.h"
#include "config.h"
#include "display/display.h"
#include "types.h"

TIME_t time = {0, 30, 9};

TimeInterval timer = TimeInterval(100, 0, true);

Display display;
Clock clock;

void setup() {
  display.begin();
  clock.begin();

  // if (!clock.ready_to_start) {
  //   display.display_err();
  //   abort();
  // }
}

void loop() {
  // clock.update(&time);

  if (timer.marked()) {
    time.minute++;

    if (time.minute > 59) {
      time.hour = (time.hour + 1) % 24;
      time.minute = 0;
    }
  }

  display.display_time(time);

  // display.display_err();
  // delay(250);
}
