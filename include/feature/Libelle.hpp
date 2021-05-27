#ifndef LIBELLE_H
#define LIBELLE_H

// Lib includes
#undef min // see https://github.com/m5stack/M5StickC/issues/132
#include <deque>

// local Includes
#include "core/Feature.hpp"

namespace CrowOs {
	namespace Feature {

		/**
		 * Small libelle feature
		 */
		class Libelle : public Core::Feature {

		private:
			/** Pointer to time helper */
			Core::Time* time;

			/** Pointer to screen helper */
			Core::Screen* screen;

			/** x accelerometer values */
			std::deque<float> accelerometerX;
			/** Y accelerometer values */
			std::deque<float> accelerometerY;

			/** X average accelerometer value */
			float accelerometerXAvg;
			/** Y average accelerometer value */
			float accelerometerYAvg;

			/** X circle position */
			int positionX;
			/** Y circle position */
			int positionY;

			/** X calibration position */
			float calibrationX;
			/** Y calibration position */
			float calibrationY;

			/** circle radius */
			int radius;

			/**
			 * Update position x and y values from MPU
			 */
			void updatePositions();

			/**
			 * Draw accelerometer circle
			 *
			 * @param clear if true than it will use background color
			 */
			void drawCircle(const bool clear = false);

			/**
			 * Draw calibration circle
			 */
			void drawCalibrationCircle();

		public:
			/**
			 * Initialise new Libelle
			 */
			Libelle();

			/**
			 * Called after Feature creation before loop when state changes to this feature
			 * You should initialise all your variables here and restore savedData to your class if savedData is not null
			 *
			 * @param screenHelper Screen helper used to manage screen
			 * @param timeHelper   Time helper used to manage upTime and sleepTime
			 * @param ledHelper    Led helper used to manage Led state
			 * @param savedData    pointer to feature persistent data
			 */
			void onStart(Core::Screen* screenHelper, Core::Time* timeHelper, Core::Led* ledHelper, DynamicJsonDocument* savedData) override;

			/**
			 * Called before Feature destroying after loop when state changes from this feature to another
			 * You should destroy all your variables here and save the persistent one into savedData pointer
			 *
			 * @param savedData pointer to feature persistent data
			 */
			void onStop(DynamicJsonDocument* savedData) override;

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
