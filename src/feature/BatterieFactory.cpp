/**
 * BatteryFactory class implementation
 * @author error23
 */
#include "feature/BatteryFactory.hpp"

namespace CrowOs {
	namespace Feature {

		/**
		 * Initialise battery factory
		 */
		BatteryFactory::BatteryFactory()
			: FeatureFactory("BatteryFactory", true) {
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
		Core::Feature* BatteryFactory::createFeature() {
			if(LOG_INFO) Serial.println("Info : [BatteryFactory] createFeature Battery");
			return new Battery();
		}

	} // namespace Feature
} // namespace CrowOs
