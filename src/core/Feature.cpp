#include "core/Feature.hpp"

/**
 * Initialise Feature and add it to static features
 */
CrowOs::Core::Feature::Feature(boolean alwaysLoop)
	: alwaysLoop(alwaysLoop) {

	features.push_back(this);
}

/**
 * indicates if feature should always alwaysLoop
 *
 * @return alwaysLoop
 */
boolean CrowOs::Core::Feature::isAlwaysLoop() const {
	return alwaysLoop;
}
