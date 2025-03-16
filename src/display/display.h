#include <Arduino.h>

#include "types.h"

class Display {
 public:
  Display();
  void begin();
  void display(TIME_t _time);

  bool colon;

 private:
  // TIME_t time;
};
