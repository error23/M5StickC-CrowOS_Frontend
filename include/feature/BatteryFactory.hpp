#ifndef BATTERY_FACTORY_H
#define BATTERY_FACTORY_H

// local Includes
#include "Battery.hpp"
#include "core/FeatureFactory.hpp"

namespace CrowOs {
	namespace Feature {

		/**
		 * Battery factory class
		 */
		class BatteryFactory : public Core::FeatureFactory {

		public:
			/**
			 * Initialise battery factory
			 */
			BatteryFactory();

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
