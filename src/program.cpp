#include "program.h"

#include "config.h"
#include "display/display.h"
#include "lamp.h"
#include "types.h"

Lamp lamp = Lamp((float[LAMP_LED_BRIGHNESS_COUNT]){0.3, 1.0});

Program::Program() : current_display(DISPLAY_STATE::STANDBY) {}

void Program::begin() {
  lamp.begin();
  lamp.update();
}

void Program::handle_lamp_event() {
  lamp.toggle_state();
  lamp.update();
}

void Program::handle_set_time_event() {
  current_display = DISPLAY_STATE::SET_TIME;
}

void Program::handle_set_alarm_event() {}

void Program::standby() { current_display = DISPLAY_STATE::STANDBY; }

void Program::display_time() {}

void Program::display_set_time() {}

bool Program::is_alarming() { return false; }

void Program::stop_alarm() {}
