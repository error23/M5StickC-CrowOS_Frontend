/**
 * OmniLevel class implementation
 */
#include "feature/OmniLevel.hpp"

namespace CrowOs {
	namespace Feature {

		/**
		 * Initialise new OmniLevel
		 */
		OmniLevel::OmniLevel()
			: Feature("OmniLevel")
			, ERROR_THRESHOLD(5.0)
			, screen(NULL)
			, led(NULL)
			, time(NULL)
			, currentPressure(0)
			, savedPressure(50) {
		}

		/**
		 * Called after Feature creation before loop when state changes to this feature
		 * You should initialise all your variables here and restore savedData to your class if savedData is not null
		 *
		 * @param screenHelper Screen helper used to manage screen
		 * @param timeHelper   Time helper used to manage upTime and sleepTime
		 * @param ledHelper    Led helper used to manage Led state
		 * @param savedData    pointer to feature persistent data
		 */
		void OmniLevel::onStart(Core::Screen* screenHelper, Core::Time* timeHelper, Core::Led* ledHelper, DynamicJsonDocument* savedData) {

			if(LOG_INFO) Serial.println("Info : [OmniLevel] onStart");

			// Strore useful helpers
			screen = screenHelper;
			led = ledHelper;
			time = timeHelper;

			// Setup screen
			screen->setScreenOrientation(Core::Screen::SCREEN_NORMAL_PORTRET);
			screen->setBackground(TFT_BLACK);
			screen->clearLCD();

			// Get saved data
			if(LOG_DEBUG) Serial.printf("Debug : [OmniLevel] onStart savedPressure = %d", savedPressure);
			if(savedData != NULL) savedPressure = (*savedData)["pressure"];
			if(LOG_DEBUG) Serial.printf(", new savedPressure = %d\n", savedPressure);

			// Draw progress bar rectangle
			M5.Lcd.drawRect(5, 17, 21, 134, 0x7bef);

			// Draw calibration line
			M5.Lcd.drawLine(1, 18 + (127 - savedPressure), 4, 18 + (127 - savedPressure), 0x7bef);
			M5.Lcd.drawLine(26, 18 + (127 - savedPressure), 29, 18 + (127 - savedPressure), 0x7bef);

			// Draw calibration instructions
			screen->clearText(9, 15, 152);
			screen->printText("Calibrate", 15, 152, TFT_CYAN);
		}

		/**
		 * Called before Feature destroying after loop when state changes from this feature to another
		 * You should destroy all your variables here and save the persistent one into savedData pointer
		 *
		 * @param savedData pointer to feature persistent data
		 */
		void OmniLevel::onStop(DynamicJsonDocument* savedData) {

			if(LOG_INFO) Serial.println("Info : [OmniLevel] onStop");
			if(savedData != NULL) (*savedData)["pressure"] = savedPressure;
			if(LOG_INFO) Serial.printf("Info : [OmniLevel] onStop savedPressure = %d\n", savedPressure);
		}

		/**
		 * Main Feature loop
		 * You should developp your main loop here
		 */
		void OmniLevel::loop() {

			currentPressure = map(analogRead(G36), 0, 3095, 0, 127);
			if(LOG_DEBUG) Serial.printf("Debug : [OmniLevel] loop currentPressure = %d, savedPressure = %d\n", currentPressure, savedPressure);

			progressBar();

			if(getPercentError(currentPressure, savedPressure) < ERROR_THRESHOLD) {
				led->on();
				M5.Lcd.pushImage(40, 50, 32, 32, thumbs_up);
			}
			else {
				led->off();

				if(currentPressure > savedPressure) {
					M5.Lcd.pushImage(40, 50, 32, 32, rotate_left);
				}
				else {
					M5.Lcd.pushImage(40, 50, 32, 32, rotate_right);
				}
			}
		}

		/**
		 * Called when home button is clicked
		 */
		void OmniLevel::onHomeClick() {

			time->keepWokedUp();
			updateCalibration(currentPressure);
		}

		/**
		 * Called when home button is double clicked
		 */
		void OmniLevel::onHomeDoubleClick() {
			time->keepWokedUp();
			updateCalibration(50);
		}

		/**
		 * Calculate error percentage between approx and exact
		 *
		 * @param approx to calculate for
		 * @param exact  to calculate for
		 *
		 * @return percentage of error between approx and exact
		 */
		float OmniLevel::getPercentError(const float approx, const float exact) const {
			return (abs(approx - exact) / exact) * 100;
		}

		/**
		 * Updates calibration from pressure to savedPressure
		 *
		 * @param pressure pressure to update with
		 */
		void OmniLevel::updateCalibration(const int pressure) {

			if(pressure == savedPressure) return;

			if(LOG_DEBUG) Serial.printf("Debug : [OmniLevel] updateCalibration new pressure = %d\n", pressure);

			// clear old line
			M5.Lcd.drawLine(1, 18 + (127 - savedPressure), 4, 18 + (127 - savedPressure), BLACK);
			M5.Lcd.drawLine(26, 18 + (127 - savedPressure), 29, 18 + (127 - savedPressure), BLACK);

			savedPressure = pressure;
			M5.Lcd.drawLine(1, 18 + (127 - savedPressure), 4, 18 + (127 - savedPressure), 0x7bef);
			M5.Lcd.drawLine(26, 18 + (127 - savedPressure), 29, 18 + (127 - savedPressure), 0x7bef);
		}

		/**
		 * Draw progress bar from currentPressure
		 */
		void OmniLevel::progressBar() {

			for(int i = currentPressure; i <= 127; i++) {
				M5.Lcd.fillRect(8, 147 - i, 15, 1, BLACK);
			}
			for(int i = 0; i <= currentPressure; i++) {
				M5.Lcd.fillRect(8, 147 - i, 15, 1, rainbow(i));
			}
			// Draw progress bar rectangle
			M5.Lcd.drawRect(5, 17, 21, 134, 0x7bef);
		}

		/**
		 * Gets progresss bar rainbow colour
		 * The value is converted to a spectrum colour from 0 = blue through to red = blue
		 *
		 * @method rainbow
		 * @param value expected to be in range 0-127
		 *
		 * @return a spectrum colour from 0 = blue through to red = blue
		 */
		unsigned int OmniLevel::rainbow(int value) {

			// Red is the top 5 bits of a 16 bit colour value
			byte red = 0;

			// Green is the middle 6 bits
			byte green = 0;

			// Blue is the bottom 5 bits
			byte blue = 0;

			byte quadrant = value / 32;

			if(quadrant == 0) {
				blue = 31;
				green = 2 * (value % 32);
				red = 0;
			}
			if(quadrant == 1) {
				blue = 31 - (value % 32);
				green = 63;
				red = 0;
			}
			if(quadrant == 2) {
				blue = 0;
				green = 63;
				red = value % 32;
			}
			if(quadrant == 3) {
				blue = 0;
				green = 63 - 2 * (value % 32);
				red = 31;
			}
			return (red << 11) + (green << 5) + blue;
		}

	} // namespace Feature
} // namespace CrowOs
