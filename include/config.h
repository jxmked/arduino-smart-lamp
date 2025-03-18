#ifndef CONFIG_h
#define CONFIG_h

// SETUPS

#define WAIT_CALL_FOR_SET_TIME 3000  // ms

// Back to standby mode when no event receive has been done for a specific
// seconds
#define INACTIVITY_INTERVAL 10000  // ms

// END SETUPS

#define LAMP_LED_PIN 5
#define LAMP_LED_BRIGHNESS_COUNT 2

#define BTN_SET_PIN 4     // Set
#define BTN_ADJUST_PIN 3  // Adjust

#define TOUCH_SENSOR_PIN 2  // A push button e.g.

#define BUZZER_PIN A3

/** 7 Segment PIN Config **/

#define SEG_D_4 9  // Digit 4
#define SEG_D_3 8  // Digit 3
#define SEG_D_2 7  // Digit 2
#define SEG_D_1 6  // Digit 1

#define SEG_L_S0 10  // A
#define SEG_L_S1 11  // B
#define SEG_L_S2 12  // C
#define SEG_L_S3 13  // D
#define SEG_L_S4 A2  // E
#define SEG_L_S5 A1  // F
#define SEG_L_S6 A0  // G

/** END 7 Segment PIN Config **/

#endif