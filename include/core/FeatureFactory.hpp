#ifndef FEATURE_FACTORY_H
#define FEATURE_FACTORY_H

// Lib includes
#include <vector>

// local Includes
#include "Defines.hpp"
#include "Feature.hpp"

// Methods

/**
 * Initialise all feature factories
 * You should implement this method in order to declare feature factories to the os
 */
void initialiseFeatureFactories();

namespace CrowOs {
	namespace Core {

		/**
		 * Abstract feature factory class
		 *
		 * Implement this class in order to declare your feature creation mehtod
		 */
		class FeatureFactory {

		private:
			/** Feature factory id in database */
			long m_id;

			/** Feature factory name */
			const char* featureFactoryName;

			/** Indicates if feature made by this factory should be always in a main loop */
			const bool alwaysLoop;

			/**
			 * Verifies if given featureFactory has same name as this one
			 *
			 * @param  featureFactory to verify
			 * @return true if featureFactory.name is same
			 */
			const bool hasSameName(const FeatureFactory* featureFactory) const;

		public:
			/** Feature factories static holder */
			static std::vector<std::pair<FeatureFactory*, String>> featureFactories;

			/**
			 * Initialise Feature factory
			 *
			 * @param featureFactoryName indicates name of this feature factory
			 * @param alwaysLoop         indicates if feature made by this factory should be always in a main loop default false
			 */
			FeatureFactory(const char* featureFactoryName, const bool alwaysLoop = false);

			/**
			 * Default destructor
			 */
			virtual ~FeatureFactory();

			/**
			 * Creates new feature
			 * You should implement this in order to initialise your Feature
			 * This method will be called before Feature.onStart method
			 * Ex:
			 * return new MyFeature();
			 *
			 * @return Feature* your feature
			 */
			virtual Feature* createFeature() = 0;

			/**
			 * Gets the feature factory id from database
			 *
			 * @return feature FeatureFactory id
			 */
			const long getFeatureFactoryId() const;

			/**
			 * Sets feature factory id in database
			 *
			 * @param id to be set
			 */
			void setFeatureFactoryId(long id);

			/**
			 * Gets feature factory name
			 *
			 * @return featureFactoryName
			 */
			const char* getFeatureFactoryName() const;

			/**
			 * Indicates if feature made by this factory should be always in a main loop
			 *
			 * @return alwayLoop boolean
			 */
			const bool isAlwaysLoop() const;
		};

	} // namespace Core
} // namespace CrowOs
#endif
