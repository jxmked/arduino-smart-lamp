#include <Arduino.h>
#include <Wire.h>

#include "Button.h"
#include "RTClib.h"
#include "config.h"

RTC_DS1307 rtc;

Button touch = Button(TOUCH_SENSOR);
Button btn_a = Button(BTN_A);
Button btn_b = Button(BTN_B);
Button btn_c = Button(BTN_C);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();

  while (!Serial);

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  touch.begin();
  btn_a.begin();
  btn_b.begin();
  btn_c.begin();
};

void loop() {}