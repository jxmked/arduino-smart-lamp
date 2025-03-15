#include <Arduino.h>

struct __emit_num_values {
  uint8_t leds[7];
  uint8_t led_count;
};

struct __emit_obj {
  uint8_t a;
  uint8_t b;
};

// HIGH-LOW
__emit_obj OBJ[3][7] = {
    // Need to fill some so with empty (0, 1) so we can normally use anything.
    {{0, 1}, {0, 1}, {1, 2}, {1, 3}, {0, 1}, {0, 1}, {0, 1}},

    // This (2, 3) are replace by (0, 1) since (2, 3) are damaged
    {{2, 0}, {0, 1}, {3, 2}, {2, 1}, {3, 1}, {3, 0}, {1, 0}},
    {{4, 1}, {3, 4}, {4, 3}, {2, 4}, {4, 2}, {1, 4}, {4, 0}}};

__emit_num_values num_combo[10] = {
    {{0, 1, 2, 3, 4, 5}, 6}, {{2, 3}, 2},       {{1, 2, 4, 5, 6}, 5},
    {{1, 2, 3, 4, 6}, 5},    {{0, 2, 3, 6}, 4}, {{0, 1, 3, 4, 6}, 5},
    {{0, 1, 3, 4, 5, 6}, 6}, {{1, 2, 3}, 3},    {{0, 1, 2, 3, 4, 5, 6}, 7},
    {{0, 1, 2, 3, 4, 6}, 6}};

int sx = 5;
int sy = 10;

uint8_t pins[5] = {8, 9, 10, 11, 12};

void reset_leds() {
  for (int i = 0; i < 5; i++) pinMode(pins[i], INPUT);
}

void combo(uint8_t a, uint8_t b) {
  a = pins[a];
  b = pins[b];

  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);

  delay(0);

  // Need everything to set to low so
  // We not disturb other leds
  digitalWrite(b, LOW);
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
}

void emit_led(uint8_t seg, uint8_t index) {
  reset_leds();

  seg = seg % 3;

  index %= 7;

  __emit_obj res = OBJ[seg][index];

  combo(res.a, res.b);
}

void emit_num(uint8_t seg, uint8_t num) {
  if (seg == 0) {
    if (num != 1) return;
  }

  __emit_num_values res_num = num_combo[num];

  for (int i = 0; i < res_num.led_count; i++) {
    uint8_t n = res_num.leds[i];

    emit_led(seg, n);

    delay(0);
  }
}

/**
 * Being used to map leds
 *
 */
void handle_serial() {
  if (Serial.available() > 0) {
    String inputString = Serial.readStringUntil('\n');
    inputString.trim();

    if (inputString.length() == 2 &&
        (inputString[0] >= '0' && inputString[0] <= '9') &&
        (inputString[1] >= '0' && inputString[1] <= '9')) {
      int tensDigit = inputString[0] - '0';  // Convert char to int
      int onesDigit = inputString[1] - '0';  // Convert char to int

      Serial.print("Tens digit: ");
      Serial.println(tensDigit);
      Serial.print("Ones digit: ");
      Serial.println(onesDigit);

      sx = tensDigit;
      sy = onesDigit;
    } else {
      Serial.println("Invalid input. Please enter a two-digit number.");
    }
  }
}

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    /* code */
  }
}

void loop() {
  uint8_t count = (millis() / 3600000) % 100;
  // handle_serial();
  // combo(sx, sy);
  emit_num(0, (count / 100) % 10);
  emit_num(1, (count / 10) % 10);
  emit_num(2, count % 10);
}