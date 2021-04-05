/**
 * Abstract Feature class implementation
 * @author error23
 */
#include "core/Feature.hpp"

namespace CrowOs {
	namespace Core {

		/**
		 * Initialise new feature
		 *
		 * @param featureName Indicates name of this feature
		 * @param indicates   if it should always loop
		 */
		Feature::Feature(const char* featureName, const bool alwaysLoop /* = false */)
			: featureName(featureName)
			, alwaysLoop(alwaysLoop) {

			if(LOG_INFO) Serial.printf("Info : feature %s created\n", featureName);
			if(LOG_DEBUG) Serial.printf("Debug : alwaysLoop = %d\n", alwaysLoop);
		}

		/**
		 * Default destructor
		 */
		Feature::~Feature() {

			if(LOG_INFO) Serial.printf("Info : feature %s deleted\n", featureName);
		}

		/**
		 * Indicate if feature is always looping dispite of actual state
		 *
		 * @return true if feature should always loop
		 */
		boolean Feature::isAlwaysLoop() const {

			return alwaysLoop;
		}
	} // namespace Core
} // namespace CrowOs
