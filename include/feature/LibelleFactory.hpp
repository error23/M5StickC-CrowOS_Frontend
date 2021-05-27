#ifndef LIBELLE_FACTORY_H
#define LIBELLE_FACTORY_H

// local Includes
#include "Libelle.hpp"
#include "core/FeatureFactory.hpp"

namespace CrowOs {
	namespace Feature {

		/**
		 * Libelle factory class
		 */
		class LibelleFactory : public Core::FeatureFactory {

		public:
			/**
			 * Initialise libelle factory
			 */
			LibelleFactory();

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
