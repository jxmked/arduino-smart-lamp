#ifndef CONFIG_h
#define CONFIG_h

#define SEVEN_SEGMENT_DIO_PIN 12
#define SEVEN_SEGMENT_CLK_PIN 13

#define LAMP_LED_PIN 9
#define LAMP_LED_BRIGHNESS_COUNT 2

#define BTN_A_PIN 6  // Hour
#define BTN_B_PIN 5  // Set
#define BTN_C_PIN 4  // Minute

#define TOUCH_SENSOR_PIN 2  // A push button e.g.

#define BUZZER_PIN 3

/** 7 Segment PIN Config **/

#define SEG_D_4 7   // Digit 4
#define SEG_D_3 8   // Digit 3
#define SEG_D_2 12  // Digit 2
#define SEG_D_1 13  // Digit 1

// Multiplexer
#define SEG_ENA 10  // Set to HIGH to disable, otherwise enable
#define SEG_SIG 11

#define SEG_L_S0 A3
#define SEG_L_S1 A2
#define SEG_L_S2 A1
#define SEG_L_S3 A0

/** END 7 Segment PIN Config **/

#endif