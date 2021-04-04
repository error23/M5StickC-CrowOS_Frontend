/**
 * Abstract FeatureFactory implementation
 * @author error23
 */
#include "core/FeatureFactory.hpp"

namespace CrowOs {
	namespace Core {

		/**
		 * Initialise Feature factory
		 */
		FeatureFactory::FeatureFactory() {
			featureFactories.push_back({this, NULL});
		}
	} // namespace Core
} // namespace CrowOs
