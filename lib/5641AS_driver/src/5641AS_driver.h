#ifndef D_5641AS_h
#define D_5641AS_h

#include <Arduino.h>

/**
 * I can't find suitable driver for 7 segment display with model
 * 5641AS that would work with multiplexer. Hoping multiplexer is
 * fast enough
 *
 * This driver accepts 7 bit input.
 *
 * 1111 1 11
 *
 * Last 2 bit are the digit selector 1-4 (00-11)
 * The 3rd bit from last are the signal (HIGH - LOW)
 * The 4 left bit are the led selector 1-8 (0000-0111)
 *
 */

struct D_5641AS_PINS {
  uint8_t D1;
  uint8_t D2;
  uint8_t D3;
  uint8_t D4;
  uint8_t S0;
  uint8_t S1;
  uint8_t S2;
  uint8_t S3;
  uint8_t SI;
  uint8_t EN;
};

class D_5641AS {
 public:
  D_5641AS(D_5641AS_PINS _pins);

  void begin();
  void reset_digits();
  void reset_leds();
  void enable();
  void disable();
  void set_signal(bool sig);
  void emit(uint8_t feed);
  void emit_num(uint16_t feed);

 private:
  D_5641AS_PINS pins;

  const static uint8_t H0x0 = 0x0;
  const static uint8_t H0x1 = 0x1;
  const static uint8_t H0x2 = 0x2;
  const static uint8_t H0x3 = 0x3;
  const static uint8_t H0x4 = 0x4;
  const static uint8_t H0x8 = 0x8;
};

#endif