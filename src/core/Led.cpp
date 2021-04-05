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
		}

		/**
		 * Set up Led helper
		 */
		void Led::setUp() {
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
		 * Toggle led on/off state
		 */
		void Led::toggle() {

			setLed(!state);
		}

		/**
		 * Set led state
		 *
		 * @param newState HIGH to turn off LOW to turn on
		 */
		void Led::setLed(const bool newState) {

			if(state == newState) return;
			state = newState;
			digitalWrite(LED_PIN, state);
		}
	} // namespace Core
} // namespace CrowOs
