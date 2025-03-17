/**
 * Button - a small library for Arduino to handle button debouncing
 *
 * MIT licensed.
 */
#ifndef DualPinButton_h
#define DualPinButton_h

#include <Arduino.h>

class DualPinButton {
 public:
  DualPinButton(uint8_t pin_a, uint8_t pin_b, uint16_t debounce_ms = 100);
  void begin();
  bool read();
  bool toggled();
  bool pressed();
  bool released();
  bool has_changed();
  uint16_t repeat_count();
  void set_repeat(uint16_t delay_ms, int16_t repeat_ms);

  const static bool PRESSED = LOW;
  const static bool RELEASED = HIGH;

 private:
  uint16_t repeats_since_press();
  uint8_t read_pins();

  uint8_t _pin_a;
  uint8_t _pin_b;
  uint16_t _delay;
  bool _state;
  uint32_t _ignore_until;
  bool _has_changed;
  uint16_t _reported_repeats;
  int16_t _repeat_delay_ms;
  int16_t _repeat_ms;
};

#endif
