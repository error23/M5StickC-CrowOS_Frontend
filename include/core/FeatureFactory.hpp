
#include "Feature.hpp"

namespace CrowOs {
	namespace Core {

		class FeatureFactory {

		private:
			/** Feature factories holder */
			std::vector<FeatureFactory*> featureFactories;

		public:
			/**
			 * Initialise Feature factory
			 */
			FeatureFactory();

			/**
			 * Creates new feature
			 * You should implement this in order to initialise your Feature
			 * This method will be called before Feature.onStart method
			 */
			virtual Feature* create() = 0;
		};

		/**
		 * Initialise all feature factories
		 * You should implement this method in order to declare feature factories to the os
		 */
		void initialiseFeatureFactories();

	} // namespace Core
} // namespace CrowOs
