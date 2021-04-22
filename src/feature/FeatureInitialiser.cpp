#include "core/FeatureFactory.hpp"
#include "feature/BatteryFactory.hpp"

/**
 * Initialise all feature factories
 * You should implement this method in order to declare feature factories to the os
 */
void initialiseFeatureFactories() {

	if(LOG_INFO) Serial.println("Info : [initialiseFeatureFactories] Initializing Factories ...");
	new CrowOs::Feature::BatteryFactory();
	if(LOG_INFO) Serial.println("Info : [initialiseFeatureFactories] Initializing Factories Done");
}
