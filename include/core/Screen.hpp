#ifndef SCREEN_H
#define SCREEN_H

// Lib includes
#include "M5StickC.h"

namespace CrowOs {
	namespace Core {

		/**
		 * Screen class
		 *
		 * Helper used to manage device screen
		 */
		class Screen {

		private:
			uint16_t backgroundColor;

		public:
			Screen();
			void printLCD(const uint16_t color, const char* text, const int x, const int y) const;
			void clearLCD() const;
			void clearLCD(const int size, const int x, const int y) const;
			void setBackground(const uint16_t color);
		};

	} // namespace Core
} // namespace CrowOs
#endif
