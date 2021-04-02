/**
 * Abstract Feature class implementation
 * @author error23
 */

#include "core/Feature.hpp"

/**
 * Initialise Feature and add it to static features
 *
 * @param alwaysLoop indicates if feature should always loop
 */
CrowOs::Core::Feature::Feature(boolean alwaysLoop)
	: alwaysLoop(alwaysLoop) {

	features.push_back(this);
}

/**
 * indicates if feature should always loop
 *
 * @return alwaysLoop
 */
boolean CrowOs::Core::Feature::isAlwaysLoop() const {
	return alwaysLoop;
}
