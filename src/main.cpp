#include <Arduino.h>
#include <Wire.h>

#include "Button.h"
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

  if (touch_lamp.pressed()) {
    lamp.toggle_state();
    lamp.update();
  }

  if(minute_btn.pressed() && !hour_btn.pressed()) {
    Serial.println("MINUTE BTN PRESSED");
  }

  if(!minute_btn.pressed() && hour_btn.pressed()) {
    Serial.println("HOUR BTN PRESSED");
  }

  if(minute_btn.pressed() && hour_btn.pressed()) {
    Serial.println("SET BTN PRESSED");
  }

  // if(minute_btn.read() == Button::RELEASED && hour_btn.read() == Button::RELEASED) {
  //   Serial.println("SET BTN PRESSED");
  // }

  // minute_btn.read();
  // hour_btn.read();

  clock.update(&time);

  display.display_time(time);
}
