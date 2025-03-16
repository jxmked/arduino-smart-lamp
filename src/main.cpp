#include <Arduino.h>
#include <TM1637.h>
#include <Wire.h>

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

void enable_digit(int d) {
  digitalWrite(SEG_D_4, HIGH);
  digitalWrite(SEG_D_3, HIGH);
  digitalWrite(SEG_D_2, HIGH);
  digitalWrite(SEG_D_1, HIGH);

  if (d == 1)
    digitalWrite(SEG_D_1, LOW);
  else if (d == 2)
    digitalWrite(SEG_D_2, LOW);
  else if (d == 3)
    digitalWrite(SEG_D_3, LOW);
  else if (d == 4)
    digitalWrite(SEG_D_4, LOW);

  delay(1);
}

void setup() {
  pinMode(SEG_D_4, OUTPUT);
  pinMode(SEG_D_3, OUTPUT);
  pinMode(SEG_D_2, OUTPUT);
  pinMode(SEG_D_1, OUTPUT);

  pinMode(SEG_ENA, OUTPUT);
  pinMode(SEG_SIG, OUTPUT);

  pinMode(SEG_L_S0, OUTPUT);
  pinMode(SEG_L_S1, OUTPUT);
  pinMode(SEG_L_S2, OUTPUT);
  pinMode(SEG_L_S3, OUTPUT);

  // Enable
  digitalWrite(SEG_ENA, LOW);

  // Disable leds
  digitalWrite(SEG_D_4, HIGH);
  digitalWrite(SEG_D_3, HIGH);
  digitalWrite(SEG_D_2, HIGH);
  digitalWrite(SEG_D_1, HIGH);

  digitalWrite(SEG_L_S0, HIGH);
  digitalWrite(SEG_L_S1, HIGH);
  digitalWrite(SEG_L_S2, HIGH);
  digitalWrite(SEG_L_S3, HIGH);
}

void loop() {
  for (int digit = 1; digit < 5; digit++) {
    enable_digit(digit);

    for (int count = 0; count < 8; count++) {
      // RESET
      digitalWrite(SEG_L_S0, (count & B0001) ? HIGH : LOW);
      digitalWrite(SEG_L_S1, (count & B0010) ? HIGH : LOW);
      digitalWrite(SEG_L_S2, (count & B0100) ? HIGH : LOW);
      digitalWrite(SEG_L_S3, (count & B1000) ? HIGH : LOW);
    }

    for (int count = 0; count < 8; count++) {
      digitalWrite(SEG_SIG, HIGH);

      // RESET
      digitalWrite(SEG_L_S0, (count & B0001) ? HIGH : LOW);
      digitalWrite(SEG_L_S1, (count & B0010) ? HIGH : LOW);
      digitalWrite(SEG_L_S2, (count & B0100) ? HIGH : LOW);
      digitalWrite(SEG_L_S3, (count & B1000) ? HIGH : LOW);

      delay(250);

      digitalWrite(SEG_SIG, LOW);
    }
  }
}