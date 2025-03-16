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

class D_5641AS {
 public:
  D_5641AS();

  void begin();
  void emit(uint8_t);
};

#endif