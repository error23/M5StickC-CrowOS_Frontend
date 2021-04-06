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
		 * @param alwaysLoop Indicates if feature made by this factory should be always in a main loop default false
		 */
		FeatureFactory::FeatureFactory(const char* featureFactoryName, const bool alwaysLoop /* = false */)
			: featureFactoryName(featureFactoryName)
			, alwaysLoop(alwaysLoop) {
			if(LOG_INFO) Serial.printf("Info : [FeatureFactory] %s created with alwaysLoop = %d\n", featureFactoryName, alwaysLoop);

			featureFactories.push_back({this, NULL});
			if(LOG_DEBUG) Serial.printf("Debug : [FeatureFactory] %s added to featureFactories actualSize = %d\n", featureFactoryName, featureFactories.size());
		}

		/**
		 * Default destructor
		 */
		FeatureFactory::~FeatureFactory() {

			std::remove_if(featureFactories.begin(), featureFactories.end(), [this](std::pair<FeatureFactory*, void*> pair) {
				if(hasSameName(pair.first)) {

					if(LOG_DEBUG) Serial.printf("Debug : [FeatureFactory] %s removed from featureFactories actualSize = %d\n", featureFactoryName, featureFactories.size());
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

		/**
		 * Indicates if feature made by this factory should be always in a main loop
		 *
		 * @return alwayLoop boolean
		 */
		const bool FeatureFactory::isAlwaysLoop() const {

			return alwaysLoop;
		}

	} // namespace Core
} // namespace CrowOs
