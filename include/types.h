#ifndef TYPES_h
#define TYPES_h

#include <Arduino.h>

typedef struct {
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
} TIME_t;

/**
 * Blink either digit 0, 1 or digit 2, 3 for idk.
 * Can be set to all digits at once.
 *
 * SET_A = digit 0, 1
 * SET_B = digit 2, 3
 */
enum class BLINKING_SET { SET_A, SET_B, ALL, NONE };

#endif