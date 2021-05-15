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
			, brightness(15)
			, m_errorDelay(0)
			, errorMessage("\0") {

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
		 * Screen loop method
		 */
		void Screen::loop() {

			unsigned int now = millis();

			if(m_errorDelay > now) {

				if(LOG_INFO) Serial.printf("Info : [Screen] loop show error message = %s for delay = %d ms\n", errorMessage, m_errorDelay - now);

				M5.Lcd.setTextColor(TFT_BLACK, TFT_RED);
				M5.Lcd.setCursor(2, 152);
				M5.Lcd.print(errorMessage);

				int emptySize = 15 - strlen(errorMessage);
				char emptyBuff[emptySize];
				for(int i = 0; i < emptySize - 1; i++) {
					emptyBuff[i] = ' ';
				}
				emptyBuff[emptySize - 1] = '\0';
				M5.Lcd.print(emptyBuff);
			}
			else if(errorMessage[0] != '\0') {

				if(LOG_INFO) Serial.printf("Info : [Screen] loop hide error message = %s\n", errorMessage);
				clearText(15, 2, 152);
				errorMessage[0] = '\0';
			}
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
		 * Shows error at the screen
		 *
		 * @param errorText  error message to show
		 * @param errorDelay time that message will be showen in ms
		 */
		void Screen::showError(const char* errorText, const unsigned int errorDelay) {
			strncpy(errorMessage, errorText, 15);
			m_errorDelay = millis() + errorDelay;
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
