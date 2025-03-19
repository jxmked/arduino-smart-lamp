#include "lamp.h"

#include <Arduino.h>

#include "config.h"

Lamp::Lamp(float _brightness[LAMP_LED_BRIGHNESS_COUNT])
    : current_state(2), brightness{0.3, 1.0} {
  for (uint8_t i = 0; i < LAMP_LED_BRIGHNESS_COUNT; i++)
    brightness[i] = _brightness[i];
}

void Lamp::begin() { pinMode(LAMP_LED_PIN, OUTPUT); }

void Lamp::update() {
  if (current_state == LAMP_LED_BRIGHNESS_COUNT) {
    digitalWrite(LAMP_LED_PIN, LOW);
  } else {
    analogWrite(LAMP_LED_PIN, brightness[current_state] * 255);
  }
}

void Lamp::toggle_state() {
  current_state++;
  current_state = current_state % (LAMP_LED_BRIGHNESS_COUNT + 1);
}