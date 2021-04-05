/**
 * Abstract FeatureFactory implementation
 * @author error23
 */
#include "core/FeatureFactory.hpp"

namespace CrowOs {
	namespace Core {

		// Initialise static featureFactories
		std::vector<std::pair<FeatureFactory*, void*>> FeatureFactory::featureFactories;

		/**
		 * Initialise Feature factory
		 *
		 * @param featureFactoryName indicates name of this feature factory
		 */
		FeatureFactory::FeatureFactory(const char* featureFactoryName)
			: featureFactoryName(featureFactoryName) {
			featureFactories.push_back({this, NULL});

			if(LOG_INFO) Serial.printf("Info : [FeatureFactory] %s created\n", featureFactoryName);
		}

		/**
		 * Default destructor
		 */
		FeatureFactory::~FeatureFactory() {

			std::remove_if(featureFactories.begin(), featureFactories.end(), [this](std::pair<FeatureFactory*, void*> pair) {
				if(hasSameName(pair.first)) {

					if(LOG_DEBUG) Serial.printf("Debug : [FeatureFactory] %s removed from featureFactories\n", featureFactoryName);
					return true;

				} else {
					return false;
				}
			});

			if(LOG_INFO) Serial.printf("Info : [FeatureFactory] %s deleted\n", featureFactoryName);
		}

		/**
		 * Verifies if given featureFactory has same name as this one
		 *
		 * @param  featureFactory to verify
		 * @return true if featureFactory.name is same
		 */
		const bool FeatureFactory::hasSameName(const FeatureFactory* featureFactory) const {

			return strcmp(featureFactoryName, featureFactory->featureFactoryName) == 0;
		}

	} // namespace Core
} // namespace CrowOs
