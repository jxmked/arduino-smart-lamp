#ifndef PROGRAM_h
#define PROGRAM_h

#include <Arduino.h>

class Program {
 public:
  Program();
  void begin();
  void handle_lamp_event();
};

#endif