#ifndef TONE_h
#define TONE_h

#include <Arduino.h>

class Tone {
 public:
  Tone();
  void begin(uint8_t _pin);
  void play_alarm();
  void stop_alarm();

  void play_click();         // Adjustment
  void play_double_click();  // use to save/set(finalize) button

 private:
  uint8_t pin;
  bool is_playing_alarm;
};

#endif