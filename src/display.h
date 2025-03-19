#ifndef DISPLAY_DISPLAY_h
#define DISPLAY_DISPLAY_h
#include <Arduino.h>

#include "SevSeg.h"
#include "types.h"

enum class ColonState { KEEP_ON, KEEP_OFF, ACTIVE };

class Display {
 public:
  Display();
  void begin();
  void display_time(TIME_t time, BLINKING_SET blinking);
  void display_err(bool with_refresh = false);

  void display_alarm_on();
  void display_alarm_off();
  void blank();

  // ColonState colon;

 private:
  SevSeg sevseg;
};

#endif