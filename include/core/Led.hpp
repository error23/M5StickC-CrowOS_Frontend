#ifndef LED_H
#define LED_H

// Lib includes
#include "M5StickC.h"

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

			/**
			 * Set led state
			 *
			 * @param newState LOW to turn off HIGH to turn on
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
		};

	} // namespace Core
} // namespace CrowOs

#endif
