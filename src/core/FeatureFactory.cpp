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
		 */
		FeatureFactory::FeatureFactory() {
			featureFactories.push_back({this, NULL});
		}
	} // namespace Core
} // namespace CrowOs
