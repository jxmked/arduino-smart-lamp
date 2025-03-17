#include <Arduino.h>
#include <Wire.h>

#include "Button.h"
#include "TimeInterval.h"
#include "clock.h"
#include "config.h"
#include "display/display.h"
#include "lamp.h"
#include "program.h"
#include "types.h"

TIME_t time = {0, 30, 9};

Display display;
Clock clock;

Button touch_lamp(TOUCH_SENSOR_PIN);
Button minute_btn(BTN_MINU_PIN);
Button hour_btn(BTN_HOUR_PIN);

Program prog;

void setup() {
  Serial.begin(9600);

  while (!Serial);

  prog.begin();

  display.begin();
  clock.begin();

  if (!clock.ready_to_start) {
    display.display_err();
    abort();
  }

  touch_lamp.begin();
  minute_btn.begin();
  hour_btn.begin();
}

void loop() {
  uint8_t __btn_min_res = digitalRead(BTN_MINU_PIN);
  uint8_t __btn_hr_res = digitalRead(BTN_HOUR_PIN);

  bool min_btn = minute_btn.pressed(__btn_min_res);
  bool hr_btn = hour_btn.pressed(__btn_hr_res);

  // Handle lamp
  if (touch_lamp.pressed()) {
    prog.handle_lamp_event();
    return;  // Do one event at once
  }
  // END HANDLE LAMP

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
