#include "core/Led.h"

/**
 * Set up led
 */
CrowOs::Core::Led::Led()
	: LED_PIN(36)
	, state(HIGH) {
	pinMode(LED_PIN, OUTPUT);
	off();
}

/**
 * Turns on led
 */
void CrowOs::Core::Led::on() {

	setLed(LOW);
}

/**
 * Turns off led
 */
void CrowOs::Core::Led::off() {

	setLed(HIGH);
}

/**
 * Toggles led state
 */
void CrowOs::Core::Led::toggle() {

	setLed(!state);
}

/**
 * set led state
 *
 * @param newState LOW to turn off HIGH to turn on
 */
void CrowOs::Core::Led::setLed(const bool newState) {

	if(state == newState) return;
	state = newState;
	digitalWrite(LED_PIN, state);
}
