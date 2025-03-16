#include <Arduino.h>
#include <TM1637.h>

#include "config.h"
#include "display/display.h"
#include "types.h"

TIME_t time;

Display display;

void setup() {
  display.begin();

  time.second = 0;
  time.minute = 19;
  time.hour = 4;
}

void loop() {
  auto ms = (millis() / 1000) % 60;

  if (ms != time.second) {
    if (time.second == 59) {
      if (time.minute == 59) {
        time.hour = (time.hour++) % 24;
      }

      time.minute = (time.minute++) % 60;
    }

    time.second = ms;
  }

  display.display_time(time);

  delay(100);
}
