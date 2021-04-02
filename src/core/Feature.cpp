/**
 * Abstract Feature class implementation
 * @author error23
 */

#include "core/Feature.hpp"

/**
 * Indicate if feature is always looping dispite of actual state
 *
 * @return true if feature should always loop
 */
boolean CrowOs::Core::Feature::isAlwaysLoop() const {
	return alwaysLoop;
}
