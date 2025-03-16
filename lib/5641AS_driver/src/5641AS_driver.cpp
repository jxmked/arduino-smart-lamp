#include <Arduino.h>

#include "5641AS_driver.h"

D_5641AS::D_5641AS(D_5641AS_PINS _pins) : pins{_pins} {}

void D_5641AS::begin() {
  // Hot wire?
  pinMode(pins.D4, OUTPUT);
  pinMode(pins.D3, OUTPUT);
  pinMode(pins.D2, OUTPUT);
  pinMode(pins.D1, OUTPUT);

  pinMode(pins.EN, OUTPUT);
  pinMode(pins.SI, OUTPUT);

  pinMode(pins.S0, OUTPUT);
  pinMode(pins.S1, OUTPUT);
  pinMode(pins.S2, OUTPUT);
  pinMode(pins.S3, OUTPUT);
}

void D_5641AS::enable() { digitalWrite(pins.EN, LOW); }

void D_5641AS::disable() { digitalWrite(pins.EN, HIGH); }

void D_5641AS::reset_digits() {
  digitalWrite(pins.D4, HIGH);
  digitalWrite(pins.D3, HIGH);
  digitalWrite(pins.D2, HIGH);
  digitalWrite(pins.D1, HIGH);
}

void D_5641AS::reset_leds() {
  digitalWrite(pins.S0, HIGH);
  digitalWrite(pins.S1, HIGH);
  digitalWrite(pins.S2, HIGH);
  digitalWrite(pins.S3, HIGH);
}

void D_5641AS::set_signal(bool sig) { digitalWrite(pins.SI, sig ? HIGH : LOW); }

void D_5641AS::emit(uint8_t feed) {
  auto digit_selector = feed & 0x3;
  auto led_selector = feed >> 3;

  // Get leds to turn on
  digitalWrite(pins.S0, ((led_selector & 0x1) > 0) ? HIGH : LOW);
  digitalWrite(pins.S1, ((led_selector & 0x2) > 0) ? HIGH : LOW);
  digitalWrite(pins.S2, ((led_selector & 0x4) > 0) ? HIGH : LOW);
  digitalWrite(pins.S3, ((led_selector & 0x8) > 0) ? HIGH : LOW);

  // Get segment to turn on
  digitalWrite(pins.D1, (digit_selector == 0x0) ? LOW : HIGH);
  digitalWrite(pins.D2, (digit_selector == 0x1) ? LOW : HIGH);
  digitalWrite(pins.D3, (digit_selector == 0x2) ? LOW : HIGH);
  digitalWrite(pins.D4, (digit_selector == 0x3) ? LOW : HIGH);

  // Set signal
  digitalWrite(pins.SI, ((feed & 0x4) > 0) ? HIGH : LOW);
}
