/**
 * LibelleFactory class implementation
 * @author error23
 */
#include "feature/LibelleFactory.hpp"

namespace CrowOs {
	namespace Feature {

		/**
		 * Initialise libelle factory
		 */
		LibelleFactory::LibelleFactory()
			: FeatureFactory("LibelleFactory") {
		}

		/**
		 * Creates new feature
		 * You should implement this in order to initialise your Feature
		 * This method will be called before Feature.onStart method
		 * Ex:
		 * return new MyFeature();
		 *
		 * @return Feature* your feature
		 */
		Core::Feature* LibelleFactory::createFeature() {
			if(LOG_INFO) Serial.println("Info : [LibelleFactory] createFeature Libelle");
			return new Libelle();
		}

	} // namespace Feature
} // namespace CrowOs
