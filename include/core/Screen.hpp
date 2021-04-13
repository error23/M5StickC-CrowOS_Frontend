#ifndef SCREEN_H
#define SCREEN_H

// Lib includes
#include "M5StickC.h"

// local Includes
#include "Defines.hpp"

// Resource includes
#include "resources/logo.hpp"

namespace CrowOs {
	namespace Core {

		/**
		 * Screen class
		 *
		 * Helper used to manage device screen
		 */
		class Screen {

		private:
			/** Actual default background color */
			uint16_t backgroundColor;

		public:
			/**
			 * Initialise screen
			 */
			Screen();

			/**
			 * Set up Screen helper
			 */
			void setUp();

			/**
			 * Prints text on the screen
			 *
			 * @param color text color
			 * @param text  text to print
			 * @param x     cursor position x
			 * @param y     cursor position y
			 */
			void printText(const uint16_t color, const char* text, const int x, const int y) const;

			/**
			 * Paint screen with background color
			 */
			void clearLCD() const;

			/**
			 * Clear screen text
			 *
			 * @param size of text to clean
			 * @param x    cursor x position
			 * @param y    cursor y position
			 */
			void clearText(const int size, const int x, const int y) const;

			/**
			 * changes screen background color
			 *
			 * @param color to set
			 */
			void setBackground(const uint16_t color);

			/**
			 * Shows crow logo
			 */
			void showLogo() const;
		};

	} // namespace Core
} // namespace CrowOs
#endif
