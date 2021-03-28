#include "core/Screen.h"

/**
 * Set up screen
 */
Screen::Screen()
	: backgroundColor(TFT_BLACK) {

	M5.Lcd.setSwapBytes(true);
	clearLCD();
}

/**
 * Prints text on lcd screen
 */
void Screen::printLCD(const uint16_t color, const char* text, const int x, const int y) {

	M5.Lcd.setTextColor(color, backgroundColor);
	M5.Lcd.setCursor(x, y);
	M5.Lcd.print(text);
}

/**
 * Clears LCD screen
 */
void Screen::clearLCD(const int size, const int x, const int y) {

	M5.Lcd.setTextColor(backgroundColor, backgroundColor);
	M5.Lcd.setCursor(x, y);

	for(int i = 0; x < size; i++) {
		M5.Lcd.print(" ");
	}
}

/**
 * Clears hole LCD screen
 */
void Screen::clearLCD() {

	M5.Lcd.fillScreen(backgroundColor);
}

/**
 * Sets default screen background color
 *
 * @param  color color to be set
 */
void Screen::setBackground(const uint16_t color) {
	backgroundColor = color;
}
