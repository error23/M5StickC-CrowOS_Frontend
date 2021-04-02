/**
 * Abstract FeatureFactory implementation
 * @author error23
 */
#include "core/FeatureFactory.hpp"

/**
 * Initialise Feature factory
 */
CrowOs::Core::FeatureFactory::FeatureFactory() {
	featureFactories.push_back(this);
}
