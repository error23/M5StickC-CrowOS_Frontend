#ifndef FEATURE_H
#define FEATURE_H

// Lib includes
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

			/** Feature id */
			long m_id;

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
			virtual void onStart(Screen* screenHelper, Time* timeHelper, Led* ledHelper, void* savedData) = 0;

			/**
			 * Called before Feature destroying after loop when state changes from this feature to another
			 * You should destroy all your variables here and save the persistent one into savedData pointer
			 * Ex :
			 * delete pointer;
			 * *((int*)savedData) = savedInteger;
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

			/**
			 * Gets the name of this feature
			 *
			 * @return featureName
			 */
			const char* getFeatureName() const;

			/**
			 * Gets the feature id in database
			 *
			 * @return feature id
			 */
			const long getFeatureId() const;

			/**
			 * Sets feature id in database
			 *
			 * @param id id to be set
			 */
			void setFeatureId(long id);
		};

	} // namespace Core

} // namespace CrowOs

#endif
