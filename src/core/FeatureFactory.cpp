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
			featureFactories[*this];
		}
	} // namespace Core
} // namespace CrowOs
