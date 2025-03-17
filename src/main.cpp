#include <Arduino.h>
#include <Wire.h>

#include "5641AS_driver.h"
#include "Button.h"
#include "TimeInterval.h"
#include "clock.h"
#include "config.h"
#include "display/display.h"
#include "lamp.h"
#include "types.h"

// TIME_t time = {0, 30, 9};

// TimeInterval timer = TimeInterval(250, 0, true);
// Lamp lamp =
//     Lamp((uint8_t)LAMP_LED_PIN, (float[LAMP_LED_BRIGHNESS_COUNT]){0.3, 1.0});

// Display display;
// Clock clock;

// Button touch_lamp(TOUCH_SENSOR_PIN);

// void setup() {
//   Serial.begin(9600);

//   while (!Serial);

//   display.begin();
//   clock.begin();

//   if (!clock.ready_to_start) {
//     display.display_err();
//     abort();
//   }

//   touch_lamp.begin();
//   lamp.update();
// }

// void loop() {
//   if (touch_lamp.pressed()) {
//     lamp.toggle_state();
//     lamp.update();
//   }

//   clock.update(&time);

//   if (timer.marked()) display.display_time(time);
// }

D_5641AS driver = D_5641AS({SEG_D_1, SEG_D_2, SEG_D_3, SEG_D_4, SEG_L_S0,
                            SEG_L_S1, SEG_L_S2, SEG_L_S3, SEG_SIG, SEG_ENA});

void setup() {
  Serial.begin(9600);

  while (!Serial);

  driver.begin();
  driver.enable();
}

void loop() {
  // for (uint16_t i = 0; i < 10; i++) {
  //   int x = 0;
  //   while (x < 1000) driver.emit_num(i);

  //   delay(5000);
  // }
  driver.emit_num(1);

  // delay(5000);

  // for (int digit = 0x0; digit < 0x4; digit++) {
  //   for (int count = 0; count < 8; count++) {
  //     uint8_t bin = count << 3;

  //     bin |= (digit | 0x4);

  //     driver.enable();
  //     driver.emit(bin);

  //     delay(500);

  //     driver.set_signal(false);
  //     driver.disable();
  //   }
  // }
}