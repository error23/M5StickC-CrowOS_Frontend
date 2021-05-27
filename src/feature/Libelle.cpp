/**
 * Libelle class implementation
 */
#include "feature/Libelle.hpp"

namespace CrowOs {
	namespace Feature {

		/**
		 * Initialise new Libelle
		 */
		Libelle::Libelle()
			: Feature("Libelle")
			, time(NULL)
			, screen(NULL)
			, backgroundColor(0x2A)
			, accelerometerX()
			, accelerometerY()
			, accelerometerXAvg(0)
			, accelerometerYAvg(0)
			, positionX(30)
			, positionY(30)
			, calibrationX(-3)
			, calibrationY(-29)
			, radius(15) {
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
		void Libelle::onStart(Core::Screen* screenHelper, Core::Time* timeHelper, Core::Led* ledHelper, DynamicJsonDocument* savedData) {

			if(LOG_INFO) Serial.println("Info : [Libelle] onStart");

			// Strore useful helpers
			screen = screenHelper;
			time = timeHelper;

			// Get saved data
			if(LOG_DEBUG) Serial.printf("Debug : [Libelle] onStart calibrationX = %f, calibrationY = %f", calibrationX, calibrationY);
			if(savedData != NULL) {
				calibrationX = (*savedData)["calibrationX"];
				calibrationY = (*savedData)["calibrationY"];
			}
			if(LOG_DEBUG) Serial.printf(", new calibrationX = %f, calibrationY = %f\n", calibrationX, calibrationY);

			// Clear screen
			screen->setScreenOrientation(Core::Screen::SCREEN_RIGHT_LANDSCAPE);
			screen->setBackground(backgroundColor);
			screen->clearLCD();
			screen->printText("Calibrate", 15, 152, TFT_CYAN);
		}

		/**
		 * Called before Feature destroying after loop when state changes from this feature to another
		 * You should destroy all your variables here and save the persistent one into savedData pointer
		 *
		 * @param savedData pointer to feature persistent data
		 */
		void Libelle::onStop(DynamicJsonDocument* savedData) {

			if(LOG_INFO) Serial.println("Info : [Libelle] onStop");

			if(savedData != NULL) {
				(*savedData)["calibrationX"] = calibrationX;
				(*savedData)["calibrationY"] = calibrationY;

				if(LOG_DEBUG) Serial.printf("Debug : [Libelle] onStop saved calibrationX = %f, calibrationY = %f\n", calibrationX, calibrationY);
			}
		}

		/**
		 * Main Feature loop
		 * You should developp your main loop here
		 */
		void Libelle::loop() {

			drawCalibrationCircle();
			drawCircle(true);
			updatePositions();
			drawCircle();
		}

		/**
		 * Called when home button is clicked
		 */
		void Libelle::onHomeClick() {

			time->keepWokedUp();

			calibrationX = -accelerometerXAvg;
			calibrationY = -accelerometerYAvg;
			if(LOG_DEBUG) Serial.printf("Debug : [Libelle] onHomeClick calibrate calibrationX = %f, calibrationY = %f\n", calibrationX, calibrationY);
		}

		/**
		 * Called when home button is double clicked
		 */
		void Libelle::onHomeDoubleClick() {

			time->keepWokedUp();

			calibrationX = -3;
			calibrationY = -29;
			if(LOG_DEBUG) Serial.printf("Debug : [Libelle] onHomeDoubleClick reset calibrate calibrationX = %f, calibrationY = %f\n", calibrationX, calibrationY);
		}

		/**
		 * Update position x and y values from MPU
		 */
		void Libelle::updatePositions() {

			float accX = 0;
			float accY = 0;
			float accZ = 0;

			// Get data from mpu
			M5.MPU6886.getAccelData(&accX, &accY, &accZ);

			// push back values
			accelerometerX.push_back(round(accY * 100) * 2);
			accelerometerY.push_back(round(accZ * 100) * 2);

			// if we reach 11 values calculate averege x and y values
			if(accelerometerX.size() > 30) {

				// pop the first value
				accelerometerX.pop_front();
				accelerometerY.pop_front();
				accelerometerXAvg = 0;
				accelerometerYAvg = 0;

				// calculate sum for each value
				for(int i = 0; i < accelerometerX.size(); i++) {

					accelerometerXAvg += accelerometerX[i];
					accelerometerYAvg += accelerometerY[i];
				}

				// calculate averege values
				accelerometerXAvg /= accelerometerX.size();
				accelerometerYAvg /= accelerometerY.size();
			}

			// Calculate x and y positions
			positionX = accelerometerXAvg + ((screen->getMaxX() / 2) + calibrationX);
			positionY = accelerometerYAvg + ((screen->getMaxY() / 2 + screen->getMinY() / 2) + calibrationY);

			// Be sure we are not out of the screen on x axis
			if(positionX + radius >= screen->getMaxX()) positionX = screen->getMaxX() - radius;
			if(positionX - radius <= 0) positionX = radius;

			// Be sure we are not out of the screen on y axis
			if(positionY + radius >= screen->getMaxY()) positionY = screen->getMaxY() - radius;
			if(positionY - radius <= screen->getMinY(positionX - radius)) positionY = screen->getMinY(positionX - radius) + radius;
			if(positionY - radius <= screen->getMinY(positionX + radius)) positionY = screen->getMinY(positionX + radius) + radius;

			if(LOG_DEBUG) Serial.printf("Debug : [Libelle] updatePositions positionX = %d, positionY = %d, accelerometerXAvg = %f, accelerometerYAvg = %f\n", positionX, positionY, accelerometerXAvg, accelerometerYAvg);
		}

		/**
		 * Draw accelerometer circle
		 *
		 * @param clear if true than it will use background color
		 */
		void Libelle::drawCircle(const bool clear /* = false */) {

			if(clear) {
				M5.Lcd.fillCircle(positionX, positionY, radius, backgroundColor);
				if(LOG_DEBUG) Serial.printf("Debug : [Libelle] drawCircle clear = true, positionX = %d, positionY = %d\n", positionX, positionY);
			}
			else {
				M5.Lcd.pushImage(positionX - radius + 1, positionY - radius + 1, 29, 28, res_circle, 0x0000);
				if(LOG_DEBUG) Serial.printf("Debug : [Libelle] drawCircle clear = false, positionX = %d, positionY = %d\n", positionX, positionY);
			}
		}

		/**
		 * Draw calibration circle
		 */
		void Libelle::drawCalibrationCircle() {

			uint16_t color = TFT_RED;
			int x = (screen->getMaxX() / 2);
			int y = (screen->getMaxY() / 2 + screen->getMinY() / 2);

			if(abs(x - positionX) < 5 && abs(y - positionY) < 5) color = TFT_GREEN;

			M5.Lcd.drawCircle(x, y, radius + 5, color);
		}

	} // namespace Feature
} // namespace CrowOs
