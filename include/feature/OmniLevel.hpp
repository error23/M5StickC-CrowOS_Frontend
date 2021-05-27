#ifndef OMNI_LEVEL_H
#define OMNI_LEVEL_H

// local Includes
#include "core/Feature.hpp"
#include "resources/r_omni_level.hpp"

namespace CrowOs {
	namespace Feature {

		/**
		 * 3D printer bed leveling tool
		 */
		class OmniLevel : public Core::Feature {

		private:
			/** Print bed is considered leveled if error goes below this (ideally 5.0-8.0) */
			const float ERROR_THRESHOLD;

			/** Pointer to screen helper */
			Core::Screen* screen;

			/** Pointer to led helper */
			Core::Led* led;

			/** Pointer to time helper */
			Core::Time* time;

			/** The var that goes between 0-127 from FSR reading */
			int currentPressure;

			/** Calibrated pressure var that goes between 0-127 from FSR reading */
			int savedPressure;

			/**
			 * Calculate error percentage between approx and exact
			 *
			 * @param approx to calculate for
			 * @param exact  to calculate for
			 *
			 * @return percentage of error between approx and exact
			 */
			float getPercentError(const float approx, const float exact) const;

			/**
			 * Updates calibration from pressure to savedPressure
			 *
			 * @param pressure pressure to update with
			 */
			void updateCalibration(const int pressure);

			/**
			 * Draw progress bar from currentPressure
			 */
			void progressBar();

			/**
			 * Gets progresss bar rainbow colour
			 * The value is converted to a spectrum colour from 0 = blue through to red = blue
			 *
			 * @method rainbow
			 * @param value expected to be in range 0-127
			 *
			 * @return a spectrum colour from 0 = blue through to red = blue
			 */
			unsigned int rainbow(int value);

		public:
			/**
			 * Initialise new OmniLevel feature
			 */
			OmniLevel();

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
