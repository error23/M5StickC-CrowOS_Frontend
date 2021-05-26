#ifndef SCREEN_H
#define SCREEN_H

// Lib includes
#include "M5StickC.h"

// local Includes
#include "Defines.hpp"

// Resource includes
#include "resources/r_logo.hpp"

namespace CrowOs {
	namespace Core {

		/**
		 * Screen class
		 *
		 * Helper used to manage device screen
		 */
		class Screen {

		private:
			/** Current default background color */
			uint16_t backgroundColor;

			/** Minimum y on the screen */
			const short MIN_Y;

			/** Current brightness */
			short brightness;

			/** Error showing delay */
			unsigned int m_errorDelay;

			/** Current screen orientation */
			short screenOrientation;

			/** Error message showing actually on the screen */
			char errorMessage[28];

		public:
			/** Screen left landscape orientation */
			static const short SCREEN_LEFT_LANDSCAPE = 1;
			/** Screen right landscape orientation */
			static const short SCREEN_RIGHT_LANDSCAPE = 3;
			/** Screen normal portret orientation */
			static const short SCREEN_NORMAL_PORTRET = 4;
			/** Screen inverse portret orientation */
			static const short SCREEN_INVERSED_PORTRET = 2;

			/**
			 * Initialise screen
			 */
			Screen();

			/**
			 * Set up Screen helper
			 */
			void setUp();

			/**
			 * Screen loop method
			 */
			void loop();

			/**
			 * Prints text on the screen
			 *
			 * @param text                text to print
			 * @param x                   cursor position x
			 * @param y                   cursor position y
			 * @param textForegroundColor text foreground color
			 * @param textBackgroundColor text background color if not set than default background color is used
			 */
			void printText(const char* text, const int x, const int y, const uint16_t textForegroundColor, const int textBackgroundColor = -1) const;

			/**
			 * Paint screen with background color
			 */
			void clearLCD() const;

			/**
			 * Clear screen text
			 *
			 * @param size       of text to clean
			 * @param x          cursor x position
			 * @param y          cursor y position
			 * @param clearColor color to clear text with
			 */
			void clearText(const int size, const int x, const int y, const int clearColor = -1) const;

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

			/**
			 * Shows error at the screen
			 *
			 * @param errorText  error message to show
			 * @param errorDelay time that message will be showen in ms
			 */
			void showError(const char* errorText, const unsigned int errorDelay);

			/**
			 * Changes current brightness
			 */
			void changeBrightness();

			/**
			 * Get maximum X screen coordinate
			 *
			 * @return maximum X screen coordinate
			 */
			int getMaxX() const;

			/**
			 * Gets maximum caracters on X axis
			 *
			 * @return X axis maximum caracters
			 */
			int getMaxXCaracters() const;

			/**
			 * Get maximum Y screen coordinate
			 *
			 * @return maximum Y screen coordinate
			 */
			int getMaxY() const;

			/**
			 * Gets maximum caracters on Y axis
			 *
			 * @return Y axis maximum caracters
			 */
			int getMaxYCaracters() const;

			/**
			 * Get minimum Y screen coordinate
			 *
			 * @return minimum Y screen coordinate
			 */
			int getMinY() const;

			/**
			 * Sets screen orientation
			 *
			 * @param screen orientation
			 */
			void setScreenOrientation(int orientation);

			/**
			 * Sets screen orientation
			 *
			 * @param screen orientation
			 */
			short getScreenOrientation() const;
		};

	} // namespace Core
} // namespace CrowOs
#endif
