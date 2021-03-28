#include "Screen.h"

/**
 * Set up screen
 */
Screen::Screen() {
	M5.Lcd.setSwapBytes(true);
}

/**
 * Prints text on lcd screen
 */
void Screen::printLCD(uint16_t color, const char* text, int x, int y) {

	M5.Lcd.setTextColor(color, BACKGROUND_COLOR);
	M5.Lcd.setCursor(x, y);
	M5.Lcd.print(text);
}

/**
 * Clears LCD screen
 */
void Screen::clearLCD(int size, int x, int y) {

	M5.Lcd.setTextColor(BACKGROUND_COLOR, BACKGROUND_COLOR);
	M5.Lcd.setCursor(x, y);

	for(x = 0; x < size; x++) {
		M5.Lcd.print(" ");
	}
}

/**
 * Clears hole LCD screen
 */
void Screen::clearLCD() {

	M5.Lcd.fillScreen(BACKGROUND_COLOR);
}
