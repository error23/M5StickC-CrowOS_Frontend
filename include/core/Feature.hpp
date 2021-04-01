#ifndef FEATURE_H
#define FEATURE_H

// Lib includes
#include "M5StickC.h"

// local Includes
#include "Led.hpp"
#include "Screen.hpp"
#include "Time.hpp"

namespace CrowOs {
	namespace Core {

		// Classes
		class Feature {

		private:
			static std::vector<Feature*> features;
			const boolean alwaysLoop;

		protected:
		public:
			Feature(const boolean alwaysLoop);
			boolean isAlwaysLoop() const;
			virtual void setUp(Screen& screen, Time& time, Led& led) = 0;
			virtual void onStart() = 0;
			virtual void onStop() = 0;
			virtual void loop() = 0;
			virtual void onHomeClick() = 0;
			virtual void onHomeDoubleClick() = 0;
		};

	} // namespace Core

} // namespace CrowOs

// Global initialisation method
void initialiseFeatures();

#endif
