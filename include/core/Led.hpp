#ifndef LED_H
#define LED_H

// Lib includes
#include "M5StickC.h"

// local Includes
#include "Defines.hpp"

namespace CrowOs {
	namespace Core {

		/**
		 * Led class
		 *
		 * Used to manage M5StickC led state
		 */
		class Led {

		private:
			/** Led pin number */
			const int LED_PIN;

			/** Actual led state */
			bool state;

			/** Last time blink */
			unsigned long lastTimeBlink;

			/**
			 * Set led state
			 *
			 * @param newState HIGH to turn off LOW to turn on
			 */
			void setLed(const bool newState);

		public:
			/**
			 * Initialise led
			 */
			Led();

			/**
			 * Set up Led helper
			 */
			void setUp();

			/**
			 * Turns on led
			 */
			void on();

			/**
			 * Turns off led
			 */
			void off();

			/**
			 * Toggle led on/off state
			 */
			void toggle();

			/**
			 * Blink the led
			 *
			 * @param delay between two blinks
			 */
			void blink(const int delay);
		};

	} // namespace Core
} // namespace CrowOs

#endif
