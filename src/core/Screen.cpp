/**
 * Screen class implementation
 * @author error23
 */
#include "core/Screen.hpp"

namespace CrowOs {
	namespace Core {

		/**
		 * Initialise screen
		 */
		Screen::Screen()
			: backgroundColor(TFT_BLACK) {
		}

		/**
		 * Set up Screen helper
		 */
		void Screen::setUp() {

			M5.Lcd.setSwapBytes(true);
			clearLCD();
		}

		/**
		 * Prints text on the screen
		 *
		 * @param color text color
		 * @param text  text to print
		 * @param x     cursor position x
		 * @param y     cursor position y
		 */
		void Screen::printText(const uint16_t color, const char* text, const int x, const int y) const {

			M5.Lcd.setTextColor(color, backgroundColor);
			M5.Lcd.setCursor(x, y);
			M5.Lcd.print(text);
		}

		/**
		 * Paint screen with background color
		 */
		void Screen::clearLCD() const {

			M5.Lcd.fillScreen(backgroundColor);
		}

		/**
		 * Prints text on the screen
		 *
		 * @param color text color
		 * @param text  text to print
		 * @param x     cursor position x
		 * @param y     cursor position y
		 */
		void Screen::clearText(const int size, const int x, const int y) const {

			M5.Lcd.setTextColor(backgroundColor, backgroundColor);
			M5.Lcd.setCursor(x, y);

			for(int i = 0; x < size; i++) {
				M5.Lcd.print(" ");
			}
		}

		/**
		 * changes screen background color
		 *
		 * @param color to set
		 */
		void Screen::setBackground(const uint16_t color) {
			backgroundColor = color;
		}
	} // namespace Core
} // namespace CrowOs
