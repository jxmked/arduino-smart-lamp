#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "RTClib.h"

#define LED_R_PIN 11
#define LED_G_PIN 10
#define LED_B_PIN 9

#define BTN_A 6
#define BTN_B 5
#define BTN_C 4
#define TOUCH_SENSOR 2  // A push button e.g.
#define BUZZER 3

#define COLOR_COMBI_COUNT 6
struct RGB_COMBI_t {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

RGB_COMBI_t color_combi[COLOR_COMBI_COUNT] = {{255, 0, 0},     {0, 255, 0},
                                              {0, 0, 255},     {255, 200, 150},
                                              {255, 255, 255}, {127, 127, 127}};
uint8_t current_color = 0;  // Index 0 - 6

class Button {
 public:
  uint8_t pin;
  Button(uint8_t _pin) {
    pin = _pin;
    pinMode(pin, INPUT_PULLUP);
  }

  bool is_high() {
    if (digitalRead(pin) == LOW) {
      if (flag) {
        flag = false;

        return true;
      }
    } else {
      flag = true;
    }

    return false;
  }

 private:
  bool flag = true;
};

RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

Button touch = Button(TOUCH_SENSOR);
Button btn_a = Button(BTN_A);
Button btn_b = Button(BTN_B);
Button btn_c = Button(BTN_C);

void emit_rgb(RGB_COMBI_t rgb) {
  analogWrite(LED_R_PIN, rgb.r);
  analogWrite(LED_G_PIN, rgb.g);
  analogWrite(LED_B_PIN, rgb.b);
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();

  while (!Serial);

  lcd.init();
  lcd.backlight();

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);

  pinMode(TOUCH_SENSOR, INPUT_PULLUP);
}

void loop() {
  if (touch.is_high()) {
    digitalWrite(LED_R_PIN, HIGH);
  } else {
    digitalWrite(LED_R_PIN, LOW);
  }

  // DateTime now = rtc.now();

  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print("Time: ");
  // lcd.print(now.hour(), DEC);
  // lcd.print(':');
  // lcd.print(now.minute(), DEC);
  // lcd.print(':');
  // lcd.print(now.second(), DEC);

  // lcd.setCursor(0, 1);
  // lcd.print("Date: ");
  // lcd.print(now.day(), DEC);
  // lcd.print('/');
  // lcd.print(now.month(), DEC);
  // lcd.print('/');
  // lcd.print(now.year(), DEC);
}