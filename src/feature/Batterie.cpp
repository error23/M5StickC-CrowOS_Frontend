/**
 * Battery class implementation
 */
#include "feature/Battery.hpp"

namespace CrowOs {
	namespace Feature {

		/**
		 * Initialise new Battery
		 */
		Battery::Battery()
			: Feature("Battery")
			, ANIMATION_DELAY(500)
			, screen(NULL)
			, led(NULL)
			, voltage(-1)
			, current(-1)
			, level(-1)
			, warningLevel(10)
			, chargingAnimationLevel(0)
			, lastAnimationChange(0) {
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
		void Battery::onStart(Core::Screen* screenHelper, Core::Time* timeHelper, Core::Led* ledHelper, DynamicJsonDocument* savedData) {

			if(LOG_INFO) Serial.println("Info : [Battery] onStart");
			screen = screenHelper;
			led = ledHelper;
		}

		/**
		 * Called before Feature destroying after loop when state changes from this feature to another
		 * You should destroy all your variables here and save the persistent one into savedData pointer
		 *
		 * @param savedData pointer to feature persistent data
		 */
		void Battery::onStop(DynamicJsonDocument* savedData) {
			if(LOG_INFO) Serial.println("Info : [Battery] onStop");
		}

		/**
		 * Main Feature loop
		 * You should developp your main loop here
		 */
		void Battery::loop() {

			voltage = M5.Axp.GetBatVoltage();
			current = M5.Axp.GetBatCurrent();
			updateBatteryLevel();
			showBatteryLevel();
			blinkLedWarning();

			if(LOG_DEBUG) Serial.printf("Debug : [Battery] loop voltage = %f, current = %f, level = %d\n", voltage, current, level);
		}

		/**
		 * Called when home button is clicked
		 */
		void Battery::onHomeClick() {
		}

		/**
		 * Called when home button is double clicked
		 */
		void Battery::onHomeDoubleClick() {
		}

		/**
		 * Calculates battery level in fonction of battery voltage and current
		 */
		void Battery::updateBatteryLevel() {

			level = (int)(((voltage - 3.0) / (4.145 - 3.0)) * 100);

			if(current > 0) {
				level = (int)(((voltage - 3.0) / (4.198 - 3.0)) * 100);
			}
			else if(current == 0) {
				level = (int)(((voltage - 3.0) / (4.179 - 3.0)) * 100);
			}
		}

		/**
		 * Shows battery level on the Screen
		 */
		void Battery::showBatteryLevel() {

			int x = 60;
			int y = 2;

			// Show icon
			if(current > 0)
				animateCharging(x, y);
			else {
				M5.Lcd.pushImage(x, y, 16, 10, getBatteryLevelIcon(level));
			}

			// Show percentage
			char output[5];
			if(level > 99) {
				snprintf(output, sizeof output, "%d%%", level);
			}
			else if(level > 9) {
				snprintf(output, sizeof output, " %d%%", level);
			}
			else {
				snprintf(output, sizeof output, "  %d%%", level);
			}

			screen->clearText(sizeof output, x - 27, y + 2);
			screen->printText(getBatteryLevelColor(), output, x - 27, y + 2);
		}

		/**
		 * Blink led if battery is under warning level
		 */
		void Battery::blinkLedWarning() const {

			if(level <= warningLevel) {
				if(LOG_DEBUG) Serial.println("Debug : [Battery] blinkLedWarning");
				led->blink(ANIMATION_DELAY);
			}
			else {
				led->off();
			}
		}

		/**
		 * Animate battery icon while charging
		 *
		 * @param x icon x position
		 * @param y icon y position
		 */
		void Battery::animateCharging(const int x, const int y) {

			if(millis() - lastAnimationChange > ANIMATION_DELAY) {
				M5.Lcd.pushImage(x, y, 16, 10, getBatteryLevelIcon(chargingAnimationLevel));
				lastAnimationChange = millis();

				switch(chargingAnimationLevel) {
				case 0:
					chargingAnimationLevel = 11;
					break;
				case 11:
					chargingAnimationLevel = 36;
					break;
				case 36:
					chargingAnimationLevel = 51;
					break;
				case 51:
					chargingAnimationLevel = 66;
					break;
				case 66:
					chargingAnimationLevel = 81;
					break;
				case 81:
					chargingAnimationLevel = 0;
					break;
				}
			}
		}

		/**
		 * Calculates battery res icon in fonction of batteryLevel
		 *
		 * @param  batteryLevel to calculate for
		 * @return image correspending icon
		 */
		const unsigned short* Battery::getBatteryLevelIcon(const int batteryLevel) const {

			if(batteryLevel > 80) return res_battery_100;
			if(batteryLevel > 65) return res_battery_80;
			if(batteryLevel > 50) return res_battery_65;
			if(batteryLevel > 35) return res_battery_50;
			if(batteryLevel > 10) return res_battery_35;
			return res_battery_10;
		}

		/**
		 * Calculates battery level color in fonction of battery current and level
		 *
		 * @return color
		 */
		const uint16_t Battery::getBatteryLevelColor() const {

			if(current > 0) {
				return BLUE;
			}
			else if(current == 0) {
				return GREEN;
			}
			else {
				if(level > 80) return 0x07FF;
				if(level > 65) return 0x07F7;
				if(level > 50) return 0x97E0;
				if(level > 35) return 0xF7E0;
				if(level > 10) return 0xFBE0;
				return 0xF800;
			}
		}

	} // namespace Feature
} // namespace CrowOs
