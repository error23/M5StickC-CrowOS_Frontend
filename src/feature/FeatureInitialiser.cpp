#include "core/FeatureFactory.hpp"
#include "feature/BatteryFactory.hpp"
#include "feature/OmniLevelFactory.hpp"

/**
 * Initialise all feature factories
 * You should implement this method in order to declare feature factories to the os
 */
void initialiseFeatureFactories() {

	if(LOG_INFO) Serial.println("Info : [initialiseFeatureFactories] Initializing Factories ...");
	new CrowOs::Feature::BatteryFactory();
	new CrowOs::Feature::OmniLevelFactory();
	if(LOG_INFO) Serial.println("Info : [initialiseFeatureFactories] Initializing Factories Done");
}
