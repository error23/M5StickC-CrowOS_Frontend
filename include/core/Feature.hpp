#ifndef FEATURE_H
#define FEATURE_H

// Lib includes
#include "ArduinoJson.h"
#include "M5StickC.h"

// local Includes
#include "Defines.hpp"
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
			/** Feature name */
			const char* featureName;

		public:
			/**
			 * Initialise new feature
			 *
			 * @param featureName Indicates name of this feature
			 */
			Feature(const char* featureName);

			/**
			 * Default destructor
			 */
			virtual ~Feature();

			/**
			 * Called after Feature creation before loop when state changes to this feature
			 * You should initialise all your variables here and restore savedData to your class if savedData is not null
			 *
			 * @param screenHelper Screen helper used to manage screen
			 * @param timeHelper   Time helper used to manage upTime and sleepTime
			 * @param ledHelper    Led helper used to manage Led state
			 * @param savedData    pointer to feature persistent data
			 */
			virtual void onStart(Screen* screenHelper, Time* timeHelper, Led* ledHelper, DynamicJsonDocument* savedData) = 0;

			/**
			 * Called before Feature destroying after loop when state changes from this feature to another
			 * You should destroy all your variables here and save the persistent one into savedData pointer
			 *
			 * @param savedData pointer to feature persistent data
			 */
			virtual void onStop(DynamicJsonDocument* savedData) = 0;

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

			/**
			 * Gets the name of this feature
			 *
			 * @return featureName
			 */
			const char* getFeatureName() const;
		};

	} // namespace Core

} // namespace CrowOs

#endif
