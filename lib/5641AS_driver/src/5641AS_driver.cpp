#include <Arduino.h>

#include "5641AS_driver.h"

uint8_t DIGIT_TO_BYTE[10] = {
    B01111110,  // 0 012345
    B00110000,  // 1 12
    B01101101,  // 2 01346
    B01111001,  // 3 01236
    B00111011,  // 4 12356
    B01011011,  // 5 02356
    B01011111,  // 6 023456
    B01110000,  // 7 012
    B01111111,  // 8 0123456
    B01111011   // 9 012356
};

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
  uint8_t digit_selector = feed & H0x3;
  uint8_t led_selector = feed >> H0x3;

  // Get leds to turn on
  digitalWrite(pins.S0, ((led_selector & H0x1) > 0) ? HIGH : LOW);
  digitalWrite(pins.S1, ((led_selector & H0x2) > 0) ? HIGH : LOW);
  digitalWrite(pins.S2, ((led_selector & H0x4) > 0) ? HIGH : LOW);
  digitalWrite(pins.S3, ((led_selector & H0x8) > 0) ? HIGH : LOW);

  // Get segment to turn on
  digitalWrite(pins.D1, (digit_selector == H0x0) ? LOW : HIGH);
  digitalWrite(pins.D2, (digit_selector == H0x1) ? LOW : HIGH);
  digitalWrite(pins.D3, (digit_selector == H0x2) ? LOW : HIGH);
  digitalWrite(pins.D4, (digit_selector == H0x3) ? LOW : HIGH);

  // Set signal
  digitalWrite(pins.SI, ((feed & H0x4) > 0) ? HIGH : LOW);
}

void D_5641AS::emit_num(uint16_t feed) {
  // masking our bits
  uint8_t num_a = (feed & 0xf) % 10;
  uint8_t num_b = ((feed >> 4) & 0xf) % 10;
  uint8_t num_c = ((feed >> 8) & 0xf) % 10;
  uint8_t num_d = ((feed >> 12) & 0xf) % 10;

  uint8_t b_num_a = DIGIT_TO_BYTE[num_a];
  uint8_t b_num_b = DIGIT_TO_BYTE[num_b];
  uint8_t b_num_c = DIGIT_TO_BYTE[num_c];
  uint8_t b_num_d = DIGIT_TO_BYTE[num_d];

  for (uint8_t i = 6; i >= 0; i--) {
    // 0 0 0 1
    if (((b_num_a >> i) & H0x1) > 0) {
      // Serial.print(i);
      // Serial.println();
      emit((6 - i) << 3 | (H0x4 | H0x3));
    } else {
      emit(0xf << 3 | (H0x0 | H0x3));
    }
    delay(0);

    // 0 0 1 0
    if (((b_num_b >> i) & H0x1) > 0) {
      emit(i << 3 | (H0x4 | H0x3));
    } else {
      emit(0xf << 3 | (H0x0 | H0x2));
    }
    delay(0);

    // 0 1 0 0
    if (((b_num_c >> i) & H0x1) > 0) {
      emit(i << 3 | (H0x4 | H0x1));
    } else {
      emit(0xf << 3 | (H0x0 | H0x1));
    }
    delay(0);

    if (((b_num_d >> i) & H0x1) > 0) {
      emit(i << 3 | (H0x4 | H0x0));
    } else {
      emit(0xf << 3 | (H0x0 | H0x0));
    }
    delay(0);
  }
}