#include "tone.h"

#include "pitches.h"

int melody[] = {NOTE_E7, NOTE_E7,  0,       NOTE_E7, 0,       NOTE_C7, NOTE_E7,
                0,       NOTE_G7,  0,       0,       0,       NOTE_G6, 0,
                0,       0,        NOTE_C7, 0,       0,       NOTE_G6, 0,
                0,       NOTE_E6,  0,       0,       NOTE_A6, 0,       NOTE_B6,
                0,       NOTE_AS6, NOTE_A6, 0,       NOTE_G6, NOTE_E7, NOTE_G7,
                NOTE_A7, 0,        NOTE_F7, NOTE_G7, 0,       NOTE_E7, 0,
                NOTE_C7, NOTE_D7,  NOTE_B6, 0,       0};

int noteDurations[] = {12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
                       12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
                       12, 12, 12, 12, 12, 12, 12, 12, 9,  9,  9,  12,
                       12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12};

#define TONE_TIME 500    /* ms */
#define TONE_SPACING 100 /* ms */

static const uint16_t TONES[] = {
    500,
    800,
};
const uint16_t NUM_TONES = sizeof(TONES) / sizeof(TONES[0]);

Tone::Tone() : playing(false), tone_index(0), last_tone_time(0) {}

void Tone::begin(uint8_t _pin) {
  pin = _pin;
  pinMode(_pin, OUTPUT);
}

void Tone::play() {
  if (!playing || last_tone_time + TONE_TIME + TONE_SPACING < millis()) {
    tone(pin, TONES[tone_index], TONE_TIME);
    tone_index = (tone_index + 1) % NUM_TONES;
    last_tone_time = millis();
  }
  playing = true;
}

void Tone::stop() {
  noTone(pin);
  tone_index = 0;
  playing = false;
}
