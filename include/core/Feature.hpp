#ifndef FEATURE_H
#define FEATURE_H

// Lib includes
#include "M5StickC.h"

// local Includes
#include "Led.hpp"
#include "Screen.hpp"
#include "Time.hpp"

namespace CrowOs {
	namespace Core {

		/**
		 * Abstract Feature class
		 *
		 * Implement this class in order to implement new features
		 */
		class Feature {

		private:
			/** Indicates if this feature should always loop */
			const boolean alwaysLoop;

		public:
			/**
			 * Initialise new feature
			 *
			 * @param indicates if it should always loop
			 */
			Feature(const boolean alwaysLoop = false);

			/**
			 * Indicate if feature is always looping dispite of actual state
			 *
			 * @return true if feature should always loop
			 */
			boolean isAlwaysLoop() const;

			/**
			 * Called after Feature creation before loop when state changes to this feature
			 * You should initialise all your variables here and restore savedData to your class
			 *
			 * @param screenHelper Screen helper used to manage screen
			 * @param timeHelper   Time helper used to manage upTime and sleepTime
			 * @param ledHelper    Led helper used to manage Led state
			 * @param savedData    pointer to feature persistent data
			 */
			virtual void onStart(Screen& screenHelper, Time& timeHelper, Led& ledHelper, void* savedData) = 0;

			/**
			 * Called before Feature destroying after loop when state changes  from this feature to another
			 * You should destroy all your variables here and save the persistent one into savedData pointer
			 *
			 * @param  savedData pointer to feature persistent data
			 */
			virtual void onStop(void* savedData) = 0;

			/**
			 * Main Feature loop
			 * You should developp your main loop here
			 */
			virtual void loop() = 0;

			/**
			 * Called when home button is clicked
			 */
			virtual void onHomeClick() = 0;

			/**
			 * Called when home button is double clicked
			 */
			virtual void onHomeDoubleClick() = 0;
		};

	} // namespace Core

} // namespace CrowOs

#endif
