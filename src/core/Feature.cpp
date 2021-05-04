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

		/**
		 * Gets the feature id in database
		 *
		 * @return feature id
		 */
		const long Feature::getFeatureId() const {
			return m_id;
		}

		/**
		 * Sets feature id in database
		 *
		 * @param id id to be set
		 */
		void Feature::setFeatureId(long id) {
			m_id = id;
		}

	} // namespace Core
} // namespace CrowOs
