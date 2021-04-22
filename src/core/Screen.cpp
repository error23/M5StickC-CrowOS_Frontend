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
			: backgroundColor(TFT_BLACK)
			, brightness(15) {

			if(LOG_INFO) Serial.println("Info : [Screen] created with backgroundColor = TFT_BLACK");
		}

		/**
		 * Set up Screen helper
		 */
		void Screen::setUp() {

			if(LOG_INFO) Serial.println("Info : [Screen] Setup ...");
			M5.Axp.ScreenBreath(brightness);
			clearLCD();
			if(LOG_INFO) Serial.println("Info : [Screen] Setup Done");
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
			if(LOG_DEBUG) Serial.printf("Debug : [Screen] printText text = %s at x = %d and y = %d\n", text, x, y);
		}

		/**
		 * Paint screen with background color
		 */
		void Screen::clearLCD() const {

			M5.Lcd.fillScreen(backgroundColor);
			if(LOG_DEBUG) Serial.println("Debug : [Screen] clearLCD");
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

			M5.Lcd.setTextColor(backgroundColor - 1, backgroundColor);
			M5.Lcd.setCursor(x, y);

			char buff[size];
			for(int i = 0; i < size - 1; i++) {
				buff[i] = ' ';
			}
			buff[size - 1] = '\0';

			M5.Lcd.print(buff);
			if(LOG_DEBUG) Serial.printf("Debug : [Screen] clearText %d caracters at x = %d, y = %d\n", size, x, y);
		}

		/**
		 * changes screen background color
		 *
		 * @param color to set
		 */
		void Screen::setBackground(const uint16_t color) {

			backgroundColor = color;
		}

		/**
		 * Shows crow logo
		 */
		void Screen::showLogo() const {
			clearLCD();
			M5.Lcd.pushImage(2, 8, 79, 144, res_logo);
			if(LOG_DEBUG) Serial.println("Debug : [Screen] showLogo at x = 2, y = 8, w = 79, h = 144");
		}

		/**
		 * Changes current brightness
		 */
		void Screen::changeBrightness() {

			brightness++;
			if(brightness > 15) brightness = 7;
			M5.Axp.ScreenBreath(brightness);
			if(LOG_DEBUG) Serial.printf("Debug : [Screen] changeBrightness brightness = %d\n", brightness);
		}

	} // namespace Core
} // namespace CrowOs
