#ifndef FEATURE_FACTORY_H
#define FEATURE_FACTORY_H

// Lib includes
#include <vector>

// local Includes
#include "Feature.hpp"

// Methods

/**
 * Initialise all feature factories
 * You should implement this method in order to declare feature factories to the os
 */
void initialiseFeatureFactories();

namespace CrowOs {
	namespace Core {

		class FeatureFactory {

		public:
			/** Feature factories holder */
			static std::vector<std::pair<FeatureFactory*, void*>> featureFactories;

			/**
			 * Initialise Feature factory
			 */
			FeatureFactory();

			/**
			 * Default destructor
			 */
			virtual ~FeatureFactory() = default;

			/**
			 * Creates new feature
			 * You should implement this in order to initialise your Feature
			 * This method will be called before Feature.onStart method
			 *
			 * @return Feature* your feature
			 */
			virtual Feature* createFeature() = 0;

			/**
			 * Create saved data container.
			 * You should implement this method in order to create a container
			 * To your Feature persistent data
			 *
			 * @method createSavedData
			 */
			virtual void* createSavedData() = 0;
		};

	} // namespace Core
} // namespace CrowOs
#endif
