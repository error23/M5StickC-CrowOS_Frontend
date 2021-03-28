#include "core/Led.h"

/**
 * Set up led
 */
Led::Led()
	: LED_PIN(36)
	, state(HIGH) {
	pinMode(LED_PIN, OUTPUT);
	off();
}

/**
 * Turns on led
 */
void Led::on() {

	setLed(LOW);
}

/**
 * Turns off led
 */
void Led::off() {

	setLed(HIGH);
}

/**
 * Toggles led state
 */
void Led::toggle() {

	setLed(!state);
}

/**
 * set led state
 *
 * @param newState LOW to turn off HIGH to turn on
 */
void Led::setLed(const bool newState) {

	if(state == newState) return;
	state = newState;
	digitalWrite(LED_PIN, state);
}
