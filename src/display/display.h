#include <Arduino.h>

#include "types.h"

enum class ColonState { KEEP_ON, KEEP_OFF, ACTIVE };

class Display {
 public:
  Display();
  void begin();
  void display_time(TIME_t _time);
  void display_err();

  ColonState colon;

  //  private:
  //   // TIME_t time;
};
