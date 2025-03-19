#include <Arduino.h>
#include <Wire.h>

#include "Button.h"
#include "TimeInterval.h"
#include "alarm.h"
#include "clock.h"
#include "config.h"
#include "display/display.h"
#include "display_state.h"
#include "lamp.h"
#include "types.h"

// Actual Time
TIME_t time = {0, 30, 9};

// Time to display
TIME_t time_to_disp;

// Press and hold set button to set time
TimeInterval call_set_time_interval =
    TimeInterval(WAIT_CALL_FOR_SET_TIME, 0, true);

// Return to standby when no buttons being pressed while in set mode or
// something (10 Seconds). This will cancel all current transactions.
TimeInterval inactive_button = TimeInterval(INACTIVITY_INTERVAL, 0, true);

TimeInterval blinking_ival = TimeInterval(500, 0, true);

TimeInterval alarm_toggle_ival = TimeInterval(3000, 0, true);
ALARM_TOGGLE_STATE alarm_toggle_state;

Display display;
Clock clock;
Alarm alarm;

Button touch_lamp(TOUCH_SENSOR_PIN);
Button adjust_btn(BTN_ADJUST_PIN);
Button set_btn(BTN_SET_PIN);

Lamp lamp = Lamp((float[LAMP_LED_BRIGHNESS_COUNT]){0.3, 1.0});

uint8_t cursor = 0;
DISPLAY_STATE current_display;
DISPLAY_STATE last_display;

static void display_switch(void);

void setup() {
  Serial.begin(9600);

  while (!Serial);

  display.begin();
  clock.begin();

  if (!clock.ready_to_start) {
    display.display_err();
    abort();
  }

  if (!clock.alarm_is_set()) {
    ALARM_EVENT_t fresh_alarm_data = {6, 30, false};

    clock.set_alarm_data(fresh_alarm_data);

    clock.alarm_is_set(true);
  }

  ALARM_EVENT_t alarm_data = clock.get_alarm_data();

  alarm.begin();
  alarm.load_data(alarm_data);

  lamp.begin();
  lamp.update();

  touch_lamp.begin();
  set_btn.begin();
  adjust_btn.begin();

  call_set_time_interval.pause();
  call_set_time_interval.reset();

  alarm_toggle_state = ALARM_TOGGLE_STATE::NONE;

  current_display = DISPLAY_STATE::STANDBY;
  last_display = DISPLAY_STATE::STANDBY;
}

void loop() {
  clock.update(&time);

  inactive_button.update();

  time_to_disp = time;

  // INACTIVE EVENT
  if (inactive_button.marked() && current_display != DISPLAY_STATE::STANDBY) {
    current_display = DISPLAY_STATE::STANDBY;
    clock.clear_additionals();
    alarm.clear_adjustments();
    cursor = 0;
  }

  // Check if alarm is due

  if (alarm.is_due(time)) {
    Serial.print("ALARMMMM");
    // SOUND PLEASE
  }

  // Handle lamp
  if (touch_lamp.pressed()) {
    // Stop alarm when the we touch the lamp
    // than toggling the lamp lights
    if (alarm.is_ringing())
      alarm.snooze();
    else {
      lamp.toggle_state();
      lamp.update();
    }
  }
  // END HANDLE LAMP

  if (alarm_toggle_state != ALARM_TOGGLE_STATE::NONE) {
    if (alarm_toggle_ival.marked()) {
      alarm_toggle_state = ALARM_TOGGLE_STATE::NONE;
      alarm_toggle_ival.pause();
      alarm_toggle_ival.reset();
    }

    // Blinking when alarm toggle to on/off
    if (blinking_ival.marked(500)) {
      if (alarm_toggle_state == ALARM_TOGGLE_STATE::TOGGLED_ON) {
        display.display_alarm_on();
      } else {
        display.display_alarm_off();
      }
    } else {
      display.blank();
    }

    // Prevent anything yet...
    return;
  }

  display_switch();

  if (blinking_ival.marked(500)) {
    if (cursor == 1)
      display.display_time(time_to_disp, BLINKING_SET::SET_B);
    else if (cursor == 2)
      display.display_time(time_to_disp, BLINKING_SET::SET_A);
    else
      display.display_time(time_to_disp, BLINKING_SET::NONE);

  } else {
    display.display_time(time_to_disp, BLINKING_SET::NONE);
  }
}

static void display_switch(void) {
  switch (current_display) {
    case DISPLAY_STATE::STANDBY: {
      cursor = 0;

      if (set_btn.read() == Button::PRESSED) {
        inactive_button.reset();

        // Set time

        // Wait for interval before calling
        // set time
        call_set_time_interval.resume();

        set_btn.has_changed();  // clear change state

        if (call_set_time_interval.marked()) {
          call_set_time_interval.pause();
          call_set_time_interval.reset();

          cursor = 1;
          current_display = DISPLAY_STATE::SET_TIME;
        }
      } else {
        call_set_time_interval.pause();
        call_set_time_interval.reset();

        if (set_btn.has_changed()) {
          inactive_button.reset();

          // Set alarm
          cursor = 1;
          current_display = DISPLAY_STATE::SET_ALARM;
        }
      }

      // Standby mode - adjust event
      // toggle alarm on/off
      if (adjust_btn.pressed()) {
        inactive_button.reset();

        ALARM_EVENT_t alarm_data = clock.get_alarm_data();

        alarm_data.enabled = !alarm_data.enabled;

        if (alarm_data.enabled) {
          alarm_toggle_state = ALARM_TOGGLE_STATE::TOGGLED_ON;
        } else {
          alarm_toggle_state = ALARM_TOGGLE_STATE::TOGGLED_OFF;
        }

        alarm_toggle_ival.reset();
        alarm_toggle_ival.resume();

        alarm.load_data(alarm_data);

        clock.set_alarm_data(alarm_data);  // Update alarm data to nvram
      }

    } break;

    case DISPLAY_STATE::SET_TIME: {
      if (set_btn.pressed()) {
        inactive_button.reset();

        if (cursor == 1) {
          cursor = 2;
        } else {
          current_display = DISPLAY_STATE::STANDBY;

          call_set_time_interval.pause();
          call_set_time_interval.reset();

          clock.set_time();
          clock.clear_additionals();

          cursor = 0;
        }
      }

      if (adjust_btn.pressed()) {
        inactive_button.reset();

        if (cursor == 1) {
          clock.increment_minute();
        } else if (cursor == 2) {
          clock.increment_hour();
        }
      }

      clock.temporary_clock(&time_to_disp);
    } break;

    case DISPLAY_STATE::SET_ALARM: {
      ALARM_EVENT_t alarm_data = alarm.get_alarm_data();

      if (set_btn.pressed()) {
        inactive_button.reset();

        if (cursor == 1) {
          cursor = 2;
        } else {
          current_display = DISPLAY_STATE::STANDBY;

          call_set_time_interval.pause();
          call_set_time_interval.reset();

          clock.set_alarm_data(alarm_data);
          alarm.clear_adjustments();
        }

        set_btn.has_changed();
      }

      if (adjust_btn.pressed()) {
        inactive_button.reset();

        if (cursor == 1) {
          alarm.increase_minute();
        } else if (cursor == 2) {
          alarm.increase_hour();
        }
      }

      time_to_disp.hour = alarm_data.hour;
      time_to_disp.minute = alarm_data.minute;

    } break;
  }
}