#include <Arduino.h>
#include <Wire.h>

#include "Button.h"
#include "TimeInterval.h"
#include "clock.h"
#include "config.h"
#include "display/display.h"
#include "display_state.h"
#include "lamp.h"
#include "program.h"
#include "types.h"

TIME_t time = {0, 30, 9};

// Press and hold set button to set time
TimeInterval call_set_time_interval =
    TimeInterval(WAIT_CALL_FOR_SET_TIME, 0, true);

// Return to standby when no buttons being pressed while in set mode or
// something (10 Seconds). This will cancel all current transactions.
TimeInterval inactive_button = TimeInterval(INACTIVITY_INTERVAL, 0, true);

TimeInterval blinking_ival = TimeInterval(500, 0, true);

Display display;
Clock clock;
BLINKING_SET blinkset;

Button touch_lamp(TOUCH_SENSOR_PIN);
Button adjust_btn(BTN_ADJUST_PIN);
Button set_btn(BTN_SET_PIN);

Program prog;

void setup() {
  Serial.begin(9600);

  while (!Serial);

  prog.begin();

  display.begin();
  clock.begin();

  if (!clock.ready_to_start) {
    display.display_err();
    abort();
  }

  touch_lamp.begin();
  set_btn.begin();
  adjust_btn.begin();

  call_set_time_interval.pause();
  call_set_time_interval.reset();

  blinkset = BLINKING_SET::NONE;
}

void loop() {
  clock.update(&time);

  // INACTIVE EVENT
  if (inactive_button.marked() &&
      prog.current_display != DISPLAY_STATE::STANDBY) {
    Serial.println("Going back to standby mode");
    prog.current_display = DISPLAY_STATE::STANDBY;
    blinkset = BLINKING_SET::NONE;
  }

  // Handle lamp
  if (touch_lamp.pressed()) {
    // Stop alarm when the we touch the lamp
    // than toggling the lamp lights
    if (prog.is_alarming())
      prog.stop_alarm();
    else
      prog.handle_lamp_event();
  }
  // END HANDLE LAMP

  if (prog.current_display == DISPLAY_STATE::STANDBY) {
    if (set_btn.read() == Button::PRESSED) {
      inactive_button.reset();

      // Set time

      // Wait for interval before calling
      // set time
      call_set_time_interval.resume();

      set_btn.has_changed();  // clear change state

      if (call_set_time_interval.marked()) {
        call_set_time_interval.pause();

        prog.handle_set_time_event();
      }
    } else {
      call_set_time_interval.pause();
      call_set_time_interval.reset();

      if (set_btn.has_changed()) {
        inactive_button.reset();

        // Set alarm
        prog.handle_set_alarm_event();
      }
    }

    if (adjust_btn.pressed()) {
      inactive_button.reset();

      return;
    }
  }

  switch (prog.current_display) {
    case DISPLAY_STATE::STANDBY:
      break;

    case DISPLAY_STATE::SET_TIME: {
      blinkset = BLINKING_SET::ALL;
    } break;

    case DISPLAY_STATE::SET_ALARM: {
      Serial.println("asdasd");
    } break;
  }

  if (blinking_ival.marked(500)) {
    display.display_time(time, blinkset);
  } else {
    display.display_time(time, BLINKING_SET::NONE);
  }
}
