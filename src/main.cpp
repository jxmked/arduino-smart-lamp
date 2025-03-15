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
uint8_t current_color = COLOR_COMBI_COUNT;  // Index 0 - 6; 6 = off

class TimeInterval {
 public:
  TimeInterval(unsigned long _interval, unsigned long _offset = 0,
               bool _autoUpdate = false) {
    interval = _interval;
    offset = _offset;
    autoUpdate = _autoUpdate;
  }

  void update() {
    time = millis() + offset;

    if (paused) lastTime = time;
  }

  bool marked(unsigned long holdMillis = 0) {
    if (autoUpdate) update();

    const auto currentTime = time;
    const unsigned long diff = currentTime - lastTime;

    if (diff >= interval) {
      if (diff - interval >= holdMillis) lastTime = currentTime;
      return true;
    }

    return false;
  }

  void pause() { paused = true; }
  void resume() { paused = true; }

 private:
  unsigned long interval = 0;
  bool autoUpdate = false;
  unsigned long lastTime = 0;
  unsigned long time = 0;
  unsigned long offset = 0;
  bool paused = false;
};

class Button {
 public:
  Button(uint8_t _pin) { pin = _pin; }

  void init() { pinMode(pin, INPUT_PULLUP); }

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
  uint8_t pin;
};

RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

Button touch = Button(TOUCH_SENSOR);
Button btn_a = Button(BTN_A);
Button btn_b = Button(BTN_B);
Button btn_c = Button(BTN_C);

void change_rgb_state() {
  current_color++;

  RGB_COMBI_t rgb;

  if (current_color > COLOR_COMBI_COUNT) {
    current_color = 0;
  }

  if (current_color < COLOR_COMBI_COUNT) {
    rgb = color_combi[current_color];
  } else {
    rgb = {0, 0, 0};
  }

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

  touch.init();
  btn_a.init();
  btn_b.init();
  btn_c.init();
};

void loop() {
  if (touch.is_high()) {
    change_rgb_state();
  }
}