#include <Arduino.h>
#include <TM1637.h>
#include <Wire.h>

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
  Serial.begin(9600);

  while (!Serial);

  display.begin();
  clock.begin();

  if (!clock.ready_to_start) {
    display.display_err();
    abort();
  }
}

void loop() {
  clock.update(&time);

  display.display_time(time);

  delay(250);
}
