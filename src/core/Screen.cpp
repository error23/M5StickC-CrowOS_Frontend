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
			, MIN_Y(15)
			, brightness(10)
			, m_errorDelay(0)
			, screenOrientation(SCREEN_NORMAL_PORTRET)
			, errorMessage("\0") {

			if(LOG_INFO) Serial.println("Info : [Screen] created with backgroundColor = TFT_BLACK, MIN_Y = 16, brightness = 10, screenOrientation = SCREEN_NORMAL_PORTRET");
		}

		/**
		 * Set up Screen helper
		 */
		void Screen::setUp() {

			if(LOG_INFO) Serial.println("Info : [Screen] Setup ...");
			M5.Axp.ScreenBreath(brightness);
			M5.Lcd.setRotation(screenOrientation);
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

				printText(errorMessage, 2, getMaxY() - 8, TFT_BLACK, TFT_RED);

				int emptySize = getMaxXCaracters() - strlen(errorMessage);
				char emptyBuff[emptySize];
				for(int i = 0; i < emptySize - 1; i++) {
					emptyBuff[i] = ' ';
				}
				emptyBuff[emptySize - 1] = '\0';
				M5.Lcd.print(emptyBuff);
			}
			else if(errorMessage[0] != '\0') {

				if(LOG_INFO) Serial.printf("Info : [Screen] loop hide error message = %s\n", errorMessage);
				clearText(getMaxXCaracters(), 2, getMaxY() - 8);
				errorMessage[0] = '\0';
			}
		}

		/**
		 * Prints text on the screen
		 *
		 * @param text                text to print
		 * @param x                   cursor position x
		 * @param y                   cursor position y
		 * @param textForegroundColor text foreground color
		 * @param textBackgroundColor text background color if not set than default background color is used
		 */
		void Screen::printText(const char* text, const int x, const int y, const uint16_t textForegroundColor, const int textBackgroundColor /* = -1 */) const {

			if(textBackgroundColor != -1) {
				M5.Lcd.setTextColor(textForegroundColor, textBackgroundColor);
			}
			else {
				M5.Lcd.setTextColor(textForegroundColor, backgroundColor);
			}

			M5.Lcd.setCursor(x, y);
			M5.Lcd.print(text);
			if(LOG_DEBUG) Serial.printf("Debug : [Screen] printText text = %s at x = %d and y = %d\n", text, x, y);
		}

		/**
		 * Paint screen with background color
		 */
		void Screen::clearLCD() const {

			M5.Lcd.fillScreen(backgroundColor);

			if(screenOrientation == SCREEN_INVERSED_PORTRET || screenOrientation == SCREEN_NORMAL_PORTRET) {
				M5.Lcd.fillRect(0, 0, getMaxX(), MIN_Y, TFT_BLACK);
			}
			else {

				M5.Lcd.fillRoundRect(-7, -5, getMaxX() / 3, MIN_Y + 5, 5, TFT_BLACK);
				M5.Lcd.fillRoundRect((getMaxX() / 3) * 2, -5, getMaxX(), MIN_Y + 5, 5, TFT_BLACK);
			}

			if(LOG_DEBUG) Serial.println("Debug : [Screen] clearLCD");
		}

		/**
		 * Clear screen text
		 *
		 * @param size       of text to clean
		 * @param x          cursor x position
		 * @param y          cursor y position
		 * @param clearColor color to clear text with
		 */
		void Screen::clearText(const int size, const int x, const int y, const int clearColor /* = -1 */) const {

			if(clearColor != -1) {
				M5.Lcd.setTextColor(clearColor - 1, clearColor);
			}
			else {
				M5.Lcd.setTextColor(backgroundColor - 1, backgroundColor);
			}
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
		void Screen::showLogo() {

			clearLCD();
			setScreenOrientation(SCREEN_NORMAL_PORTRET);
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

			strncpy(errorMessage, errorText, getMaxXCaracters());
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

		/**
		 * Get maximum X screen coordinate
		 *
		 * @return maximum X screen coordinate
		 */
		int Screen::getMaxX() const {

			if(screenOrientation == SCREEN_INVERSED_PORTRET || screenOrientation == SCREEN_NORMAL_PORTRET) return TFT_WIDTH;
			return TFT_HEIGHT;
		}

		/**
		 * Gets maximum caracters on X axis
		 *
		 * @return X axis maximum caracters
		 */
		int Screen::getMaxXCaracters() const {

			if(screenOrientation == SCREEN_INVERSED_PORTRET || screenOrientation == SCREEN_NORMAL_PORTRET) return 15;
			return 27;
		}

		/**
		 * Get maximum Y screen coordinate
		 *
		 * @return maximum Y screen coordinate
		 */
		int Screen::getMaxY() const {

			if(screenOrientation == SCREEN_INVERSED_PORTRET || screenOrientation == SCREEN_NORMAL_PORTRET) return TFT_HEIGHT;
			return TFT_WIDTH;
		}

		/**
		 * Gets maximum caracters on Y axis
		 *
		 * @return Y axis maximum caracters
		 */
		int Screen::getMaxYCaracters() const {

			if(screenOrientation == SCREEN_INVERSED_PORTRET || screenOrientation == SCREEN_NORMAL_PORTRET) return 19;
			return 9;
		}

		/**
		 * Get minimum Y screen coordinate
		 *
		 * @param x value of x that you want min y for
		 * @return minimum Y screen coordinate
		 */
		int Screen::getMinY(int x /* = -1 */) const {

			if(screenOrientation == SCREEN_NORMAL_PORTRET || screenOrientation == SCREEN_INVERSED_PORTRET || x == -1) return MIN_Y;
			if(x > (getMaxX() / 3) - 7 && x < (getMaxX() / 3) * 2) return 0;
			return MIN_Y;
		}

		/**
		 * Sets screen orientation
		 *
		 * @param screen orientation
		 */
		void Screen::setScreenOrientation(int orientation) {

			if(orientation == screenOrientation) return;
			if(LOG_DEBUG) Serial.printf("Debug : [Screen] setScreenOrientation new orientation = %d\n", orientation);
			screenOrientation = orientation;
			M5.Lcd.setRotation(screenOrientation);
		}

		/**
		 * Sets screen orientation
		 *
		 * @param screen orientation
		 */
		short Screen::getScreenOrientation() const {

			return screenOrientation;
		}

	} // namespace Core
} // namespace CrowOs
