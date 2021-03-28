#ifndef LED_H
#define LED_H

// Includes
#include "M5StickC.h"

namespace CrowOs {
	namespace Core {

		// Classes
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
