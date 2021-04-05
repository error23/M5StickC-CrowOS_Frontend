/**
 * Led class implementation
 * @author error23
 */
#include "core/Led.hpp"

namespace CrowOs {
	namespace Core {

		/**
		 * Initialise led
		 */
		Led::Led()
			: LED_PIN(M5_LED)
			, state(LOW) {

			if(LOG_INFO) Serial.printf("Info : Led created with pin = %d and state = %d\n", LED_PIN, state);
		}

		/**
		 * Set up Led helper
		 */
		void Led::setUp() {

			pinMode(LED_PIN, OUTPUT);
			off();
			if(LOG_INFO) Serial.println("Info : Led Setup Done");
		}

		/**
		 * Turns on led
		 */
		void Led::on() {

			if(LOG_DEBUG) Serial.println("Debug : Led on");
			setLed(LOW);
		}

		/**
		 * Turns off led
		 */
		void Led::off() {

			if(LOG_DEBUG) Serial.println("Debug : Led off");
			setLed(HIGH);
		}

		/**
		 * Toggle led on/off state
		 */
		void Led::toggle() {

			if(LOG_DEBUG) Serial.printf("Debug : Led toggle state = %d, newState = %d\n", state, !state);
			setLed(!state);
		}

		/**
		 * Set led state
		 *
		 * @param newState HIGH to turn off LOW to turn on
		 */
		void Led::setLed(const bool newState) {

			if(state == newState) {
				if(LOG_DEBUG) Serial.printf("Debug : Led setLed inchanged state = %d, newState = %d\n", state, newState);
				return;
			}

			state = newState;
			digitalWrite(LED_PIN, state);
			if(LOG_DEBUG) Serial.printf("Debug : Led setLed state = %d, newState = %d\n", state, newState);
		}
	} // namespace Core
} // namespace CrowOs
