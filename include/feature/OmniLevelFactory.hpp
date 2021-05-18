#ifndef OMNI_LEVEL_FACTORY_H
#define OMNI_LEVEL_FACTORY_H

// local Includes
#include "OmniLevel.hpp"
#include "core/FeatureFactory.hpp"

namespace CrowOs {
	namespace Feature {

		/**
		 * OmniLevel factory class
		 */
		class OmniLevelFactory : public Core::FeatureFactory {

		public:
			/**
			 * Initialise OmniLevel factory
			 */
			OmniLevelFactory();

			/**
			 * Creates new feature
			 * You should implement this in order to initialise your Feature
			 * This method will be called before Feature.onStart method
			 * Ex:
			 * return new MyFeature();
			 *
			 * @return Feature* your feature
			 */
			Core::Feature* createFeature() override;
		};

	} // namespace Feature
} // namespace CrowOs
#endif
