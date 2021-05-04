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

		/**
		 * Gets the name of this feature
		 *
		 * @return featureName
		 */
		const char* Feature::getFeatureName() const {
			return featureName;
		}

	} // namespace Core
} // namespace CrowOs
