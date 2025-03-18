#ifndef PROGRAM_h
#define PROGRAM_h

#include <Arduino.h>

#include "display_state.h"
class Program {
 public:
  Program();
  void begin();
  void handle_lamp_event();
  void handle_set_time_event();
  void handle_set_alarm_event();
  void display_time();
  void display_set_time();
  bool is_alarming();
  void stop_alarm();
  DISPLAY_STATE current_display;

 private:
};

#endif