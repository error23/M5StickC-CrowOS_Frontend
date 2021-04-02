/**
 * Abstract FeatureFactory implementation
 * @author error23
 */
#include "core/FeatureFactory.hpp"

/**
 * Initialise Feature factory
 */
namespace CrowOs {
	namespace Core {
		FeatureFactory::FeatureFactory() {
			featureFactories[*this] = 0;
		}
	} // namespace Core
} // namespace CrowOs
