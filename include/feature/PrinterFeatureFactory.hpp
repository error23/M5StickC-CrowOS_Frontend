#ifndef PRINTER_FEATURE_FACTORY_H
#define PRINTER_FEATURE_FACTORY_H

// local Includes
#include "PrinterFeature.hpp"
#include "core/FeatureFactory.hpp"

namespace CrowOs {
	namespace Feature {

		/**
		 * Printer feature factory factory class
		 */
		class PrinterFeatureFactory : public Core::FeatureFactory {

		public:
			/**
			 * Initialise printer feature factory
			 */
			PrinterFeatureFactory();

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
