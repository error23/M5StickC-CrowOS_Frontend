/**
 * PrinterFeatureFactory class implementation
 * @author error23
 */
#include "feature/PrinterFeatureFactory.hpp"

namespace CrowOs {
	namespace Feature {

		/**
		 * Initialise printer feature factory
		 */
		PrinterFeatureFactory::PrinterFeatureFactory()
			: FeatureFactory("PrinterFeatureFactory") {
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
		Core::Feature* PrinterFeatureFactory::createFeature() {
			if(LOG_INFO) Serial.println("Info : [PrinterFeatureFactory] createFeature PrinterFeature");
			return new PrinterFeature();
		}

	} // namespace Feature
} // namespace CrowOs
