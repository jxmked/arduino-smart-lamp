#include <Arduino.h>
#include <TM1637.h>

#include "types.h"

enum class ColonState { KEEP_ON, KEEP_OFF, ACTIVE };

class Display {
 public:
  Display();
  void begin();
  void display_time(TIME_t time);
  void display_err();

  ColonState colon;

 private:
  TM1637 tm;
};
