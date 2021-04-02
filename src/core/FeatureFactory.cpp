/**
 * Abstract FeatureFactory implementation
 * @author error23
 */
#include "core/FeatureFactory.hpp"

CrowOs::Core::FeatureFactory::FeatureFactory() {
	featureFactories.push_back(this);
}
