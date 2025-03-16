#include "lamp.h"

#include <Arduino.h>

#include "config.h"

Lamp::Lamp(uint8_t _pin, float _brightness[LAMP_LED_BRIGHNESS_COUNT])
    : current_state(2), pin(_pin), brightness{0.3, 1.0} {
  for (uint8_t i = 0; i < LAMP_LED_BRIGHNESS_COUNT; i++)
    brightness[i] = _brightness[i];
}

void Lamp::begin() { pinMode(pin, OUTPUT); }

void Lamp::update() {
  if (current_state == LAMP_LED_BRIGHNESS_COUNT) {
    digitalWrite(pin, LOW);
  } else {
    analogWrite(pin, brightness[current_state] * 255);
  }
}

void Lamp::toggle_state() {
  current_state++;

  current_state = current_state % (LAMP_LED_BRIGHNESS_COUNT + 1);
  Serial.print("Lamp from pin ");
  Serial.print(pin);
  Serial.print(" state changed: ");
  Serial.print(current_state);
  Serial.print(" with value: ");
  Serial.println(brightness[current_state] * 255);
}