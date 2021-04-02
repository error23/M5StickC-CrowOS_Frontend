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
			/** Set led new state */
			void setLed(const bool newState);

		public:
			/**
			 * Initialise led
			 */
			Led();

			/**
			 * Turn led on
			 */
			void on();

			/**
			 * Turn led off
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
