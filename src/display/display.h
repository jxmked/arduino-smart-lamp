#include <Arduino.h>

#include "SevSeg.h"
#include "types.h"

enum class ColonState { KEEP_ON, KEEP_OFF, ACTIVE };

class Display {
 public:
  Display();
  void begin();
  void display_time(TIME_t time);
  void display_err(bool with_refresh = false);

  // ColonState colon;

 private:
  SevSeg sevseg;
};
