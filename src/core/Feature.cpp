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
		 */
		Feature::Feature(const char* featureName)
			: featureName(featureName) {

			if(LOG_INFO) Serial.printf("Info : [Feature] %s created\n", featureName);
		}

		/**
		 * Default destructor
		 */
		Feature::~Feature() {

			if(LOG_INFO) Serial.printf("Info : [Feature] %s deleted\n", featureName);
		}

	} // namespace Core
} // namespace CrowOs
