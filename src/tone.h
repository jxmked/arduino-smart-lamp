#ifndef TONE_h
#define TONE_h

#include <Arduino.h>

class Tone {
 public:
  Tone();
  void begin(uint8_t _pin);
  void play();
  void stop();

 private:
  uint8_t pin;
  bool playing;
  uint8_t tone_index;
  unsigned long last_tone_time;
};

#endif