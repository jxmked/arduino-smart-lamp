#include "program.h"

#include "config.h"
#include "display/display.h"
#include "lamp.h"
#include "types.h"

Lamp lamp = Lamp((float[LAMP_LED_BRIGHNESS_COUNT]){0.3, 1.0});

Program::Program() {}

void Program::begin() {
  lamp.begin();
  lamp.update();
}

void Program::handle_lamp_event() {
  lamp.toggle_state();
  lamp.update();
}