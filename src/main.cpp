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
#include "tone.h"
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

TimeInterval buzzer_ival = TimeInterval(100, 0, true);

Display display;
Clock clock;
Alarm alarm;

Button touch_lamp(TOUCH_SENSOR_PIN);
Button adjust_btn(BTN_ADJUST_PIN);
Button set_btn(BTN_SET_PIN);

Lamp lamp = Lamp((float[LAMP_LED_BRIGHNESS_COUNT]){0.3, 1.0});
Tone tone_alarm;

uint8_t cursor = 0;
DISPLAY_STATE current_display;
DISPLAY_STATE last_display;

static void display_switch(void);
static void stop_alarm(void);

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
    // Set the build time as our current alarm
    const char* timeStr = __TIME__;
    uint8_t build_hour = (timeStr[0] - '0') * 10 + (timeStr[1] - '0');
    uint8_t build_minute =
        ((timeStr[3] - '0') * 10 + (timeStr[4] - '0') + 1) % 60;

    ALARM_EVENT_t fresh_alarm_data = {build_hour, build_minute, false};

    clock.set_alarm_data(fresh_alarm_data);

    clock.alarm_is_set(true);
  }

  ALARM_EVENT_t alarm_data = clock.get_alarm_data();

  alarm.begin();
  alarm.load_data(alarm_data);
  alarm.set_day(clock.get_day());
  alarm.activate();

  lamp.begin();
  lamp.update();

  tone_alarm.begin(BUZZER_PIN);

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
  alarm.update(time);

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

  if (alarm.is_due() || alarm.is_ringing()) {
    cursor = 3;
    tone_alarm.play();
  }

  // Handle lamp
  if (touch_lamp.pressed()) {
    // Stop alarm when the we touch the lamp
    // than toggling the lamp lights
    if (alarm.is_ringing()) {
      stop_alarm();
    } else {
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
    else if (cursor == 3)
      display.display_time(time_to_disp, BLINKING_SET::ALL);
    else
      display.display_time(time_to_disp, BLINKING_SET::NONE);

  } else {
    display.display_time(time_to_disp, BLINKING_SET::NONE);
  }
}

static void stop_alarm(void) {
  alarm.snooze();
  tone_alarm.stop();

  cursor = 0;
}

static void display_switch(void) {
  switch (current_display) {
    case DISPLAY_STATE::STANDBY: {
      cursor = 0;

      if (set_btn.read() == Button::PRESSED) {
        if (alarm.is_ringing()) {
          stop_alarm();
          set_btn.has_changed();
          return;
        }

        inactive_button.reset();

        // Set time

        // Wait for interval before calling
        // set time
        call_set_time_interval.resume();

        set_btn.has_changed();  // clear change state

        if (call_set_time_interval.marked()) {
          tone_alarm.click();
          inactive_button.reset();

          call_set_time_interval.pause();
          call_set_time_interval.reset();

          cursor = 1;
          current_display = DISPLAY_STATE::SET_TIME;
        }
      } else {
        call_set_time_interval.pause();
        call_set_time_interval.reset();

        if (set_btn.has_changed() && last_display != DISPLAY_STATE::SET_ALARM) {
          tone_alarm.click();

          inactive_button.reset();

          // Set alarm
          cursor = 1;
          current_display = DISPLAY_STATE::SET_ALARM;
        } else {
          last_display = DISPLAY_STATE::STANDBY;
        }
      }

      // Standby mode - adjust event
      // toggle alarm on/off
      if (adjust_btn.pressed()) {
        tone_alarm.click();

        if (alarm.is_ringing()) {
          stop_alarm();
          return;
        }

        inactive_button.reset();

        ALARM_EVENT_t alarm_data = clock.get_alarm_data();

        alarm_data.enabled = !alarm_data.enabled;

        if (alarm_data.enabled) {
          alarm.activate();
          alarm_toggle_state = ALARM_TOGGLE_STATE::TOGGLED_ON;
        } else {
          alarm.deactivate();
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
        tone_alarm.click();

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
        tone_alarm.click();

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
        tone_alarm.click();

        inactive_button.reset();

        if (cursor == 1) {
          cursor = 2;
        } else {
          current_display = DISPLAY_STATE::STANDBY;
          last_display = DISPLAY_STATE::SET_ALARM;

          call_set_time_interval.pause();
          call_set_time_interval.reset();

          clock.set_alarm_data(alarm_data);

          alarm.load_data(alarm_data);

          alarm.clear_adjustments();

          clock.alarm_is_set(true);

          // check if the alarm is
          // due or not
          if (time.hour <= alarm_data.hour &&
              time.minute <= alarm_data.minute) {
            alarm.set_day(time.day);
          } else {
            alarm.set_day(time.day + 1);
          }

          cursor = 0;
        }

        set_btn.has_changed();
      }

      if (adjust_btn.pressed()) {
        tone_alarm.click();

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