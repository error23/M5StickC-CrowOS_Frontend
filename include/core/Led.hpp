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
			const int LED_PIN;
			bool state;
			void setLed(const bool newState);

		public:
			Led();
			void on();
			void off();
			void toggle();
		};

	} // namespace Core
} // namespace CrowOs

#endif
