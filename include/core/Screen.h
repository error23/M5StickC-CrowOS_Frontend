#ifndef SCREEN_H
#define SCREEN_H

// Includes
#include "M5StickC.h"

namespace CrowOs {
	namespace Core {

		// Classes
		class Screen {

		private:
			uint16_t backgroundColor;

		public:
			Screen();
			void printLCD(const uint16_t color, const char* text, const int x, const int y);
			void clearLCD();
			void clearLCD(const int size, const int x, const int y);
			void setBackground(const uint16_t color);
		};

	} // namespace Core
} // namespace CrowOs
#endif
