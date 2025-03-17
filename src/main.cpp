#include <Arduino.h>
#include <Wire.h>

#include "Button.h"
#include "Dual-pin-Button.h"
#include "TimeInterval.h"
#include "clock.h"
#include "config.h"
#include "display/display.h"
#include "lamp.h"
#include "types.h"

TIME_t time = {0, 30, 9};

TimeInterval timer = TimeInterval(250, 0, true);
Lamp lamp =
    Lamp((uint8_t)LAMP_LED_PIN, (float[LAMP_LED_BRIGHNESS_COUNT]){0.3, 1.0});

Display display;
Clock clock;

Button touch_lamp(TOUCH_SENSOR_PIN);
Button minute_btn(BTN_MINU_PIN);
Button hour_btn(BTN_HOUR_PIN);

// DualPinButton set_btn(BTN_HOUR_PIN, BTN_MINU_PIN);

void setup() {
  Serial.begin(9600);

  while (!Serial);

  display.begin();
  clock.begin();

  if (!clock.ready_to_start) {
    display.display_err();
    abort();
  }

  touch_lamp.begin();
  lamp.update();

  minute_btn.begin();
  hour_btn.begin();
}

void loop() {
  uint8_t __btn_min_res = digitalRead(BTN_MINU_PIN);
  uint8_t __btn_hr_res = digitalRead(BTN_HOUR_PIN);

  bool min_btn = minute_btn.pressed(digitalRead(BTN_MINU_PIN));
  bool hr_btn = hour_btn.pressed(digitalRead(BTN_HOUR_PIN));

  if (min_btn && hr_btn) {
    Serial.println("SET BTN PRESSED");
  } else {
    if (min_btn) {
      Serial.println("MINUTE BTN PRESSED");
    }

    if (hr_btn) {
      Serial.println("Hour BTN PRESSED");
    }
  }

  clock.update(&time);

  display.display_time(time);
}
