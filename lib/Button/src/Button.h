/**
 * Button - a small library for Arduino to handle button debouncing
 * 
 * MIT licensed.
*/
#ifndef Button_h
#define Button_h

#include <Arduino.h>

class Button {
public:
	Button(uint8_t pin, uint16_t debounce_ms = 100);
	void begin();
	bool read(uint8_t digital_data);
	bool toggled(uint8_t digital_data);
	bool pressed(uint8_t digital_data);
	bool released(uint8_t digital_data);
	bool has_changed();
	uint16_t repeat_count();
	void set_repeat(uint16_t delay_ms, int16_t repeat_ms);

	const static bool PRESSED = LOW;
	const static bool RELEASED = HIGH;

private:
	uint16_t repeats_since_press();

	uint8_t _pin;
	uint16_t _delay;
	bool _state;
	uint32_t _ignore_until;
	bool _has_changed;
	uint16_t _reported_repeats;
	int16_t _repeat_delay_ms;
	int16_t _repeat_ms;
};

#endif
