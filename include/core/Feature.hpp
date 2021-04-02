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
			 * Called every time before loop when state changes to this feature
			 * You should initialise all your variables here
			 */
			virtual void onStart(Screen* screen, Time* time, Led* led) = 0;

			/**
			 * Called when state is changed from this feature to another
			 * You should destroy all your variables here and save them
			 */
			virtual void onStop() = 0;

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
