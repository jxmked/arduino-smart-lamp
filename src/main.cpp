#include <Arduino.h>
#include <TM1637.h>

#include "config.h"
#include "types.h"

TM1637 tm(SEVEN_SEGMENT_CLK_PIN, SEVEN_SEGMENT_DIO_PIN);

TIME_t time;

void setup() {
  tm.init();

  uint32_t* data = &time;

  tm.setBrightnessPercent(100);
}

unsigned int counter = 0;

void loop() {
  // tm.display(0, (counter / 1000) % 10);
  // tm.display(1, (counter / 100) % 10);
  // tm.display(2, (counter / 10) % 10);
  tm.display(counter, true, false, 0);

  counter++;
  if (counter == 10000) {
    counter = 0;
  }

  delay(100);
}
