/**
 * Screen class implementation
 * @author error23
 */

#include "core/Screen.hpp"

/**
 * Initialise screen
 */
CrowOs::Core::Screen::Screen()
	: backgroundColor(TFT_BLACK) {

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
void CrowOs::Core::Screen::printText(const uint16_t color, const char* text, const int x, const int y) const {

	M5.Lcd.setTextColor(color, backgroundColor);
	M5.Lcd.setCursor(x, y);
	M5.Lcd.print(text);
}

/**
 * Paint screen with background color
 */
void CrowOs::Core::Screen::clearLCD() const {

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
void CrowOs::Core::Screen::clearText(const int size, const int x, const int y) const {

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
void CrowOs::Core::Screen::setBackground(const uint16_t color) {
	backgroundColor = color;
}
