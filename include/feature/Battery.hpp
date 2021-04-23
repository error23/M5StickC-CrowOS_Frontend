#ifndef BATTERY_H
#define BATTERY_H

// local Includes
#include "core/Feature.hpp"
#include "resources/r_battery.hpp"

namespace CrowOs {
	namespace Feature {

		/**
		 * System alimentation management class
		 */
		class Battery : public Core::Feature {

		private:
			/** Blink and animations delay */
			const int ANIMATION_DELAY;

			/** Pointer to screen helper */
			Core::Screen* screen;

			/** Pointer to led helper */
			Core::Led* led;

			/** Battery voltage */
			float voltage;

			/** Battery current */
			float current;

			/** Battery level */
			int level;

			/** Battery warning level */
			int warningLevel;

			/** Battery charging animation level */
			int chargingAnimationLevel;

			/** Last time animation has changed */
			unsigned long lastAnimationChange;

			/**
			 * Calculates battery level in fonction of battery voltage and current
			 */
			void updateBatteryLevel();

			/**
			 * Shows battery level on the Screen
			 */
			void showBatteryLevel();

			/**
			 * Blink led if battery is under warning level
			 */
			void blinkLedWarning() const;

			/**
			 * Animate battery icon while charging
			 *
			 * @param x icon x position
			 * @param y icon y position
			 */
			void animateCharging(const int x, const int y);

			/**
			 * Calculates battery res icon in fonction of batteryLevel
			 *
			 * @param  batteryLevel to calculate for
			 * @return image correspending icon
			 */
			const unsigned short* getBatteryLevelIcon(const int batteryLevel) const;

			/**
			 * Calculates battery level color in fonction of battery current and level
			 *
			 * @return color
			 */
			const uint16_t getBatteryLevelColor() const;

		public:
			/**
			 * Initialise new Battery
			 */
			Battery();

			/**
			 * Called after Feature creation before loop when state changes to this feature
			 * You should initialise all your variables here and restore savedData to your class it savedData is not null
			 * Ex :
			 * 	if(savedData != NULL) {
			 *		savedInteger = *((int*)savedData);
			 *	}
			 *
			 * @param screenHelper Screen helper used to manage screen
			 * @param timeHelper   Time helper used to manage upTime and sleepTime
			 * @param ledHelper    Led helper used to manage Led state
			 * @param savedData    pointer to feature persistent data
			 */
			void onStart(Core::Screen* screenHelper, Core::Time* timeHelper, Core::Led* ledHelper, void* savedData) override;

			/**
			 * Called before Feature destroying after loop when state changes from this feature to another
			 * You should destroy all your variables here and save the persistent one into savedData pointer
			 * Ex :
			 * delete pointer;
			 * *((int*)savedData) = savedInteger;
			 *
			 * @param  savedData pointer to feature persistent data
			 */
			void onStop(void* savedData) override;

			/**
			 * Main Feature loop
			 * You should developp your main loop here
			 */
			void loop() override;

			/**
			 * Called when home button is clicked
			 */
			void onHomeClick() override;

			/**
			 * Called when home button is double clicked
			 */
			void onHomeDoubleClick() override;
		};

	} // namespace Feature
} // namespace CrowOs

#endif
