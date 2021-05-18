/**
 * OmniLevelFactory class implementation
 * @author error23
 */
#include "feature/OmniLevelFactory.hpp"

namespace CrowOs {
	namespace Feature {

		/**
		 * Initialise OmniLevelFactory factory
		 */
		OmniLevelFactory::OmniLevelFactory()
			: FeatureFactory("OmniLevelFactory") {
		}

		/**
		 * Creates new feature
		 * You should implement this in order to initialise your Feature
		 * This method will be called before Feature.onStart method
		 * Ex:
		 * return new MyFeature();
		 *
		 * @return Feature* your feature
		 */
		Core::Feature* OmniLevelFactory::createFeature() {
			if(LOG_INFO) Serial.println("Info : [OmniLevelFactory] createFeature OmniLevel");
			return new OmniLevel();
		}

	} // namespace Feature
} // namespace CrowOs
