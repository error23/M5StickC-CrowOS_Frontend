/**
 * Abstract Feature class implementation
 * @author error23
 */

#include "core/Feature.hpp"

/**
 * indicates if feature should always loop
 *
 * @return alwaysLoop
 */
boolean CrowOs::Core::Feature::isAlwaysLoop() const {
	return alwaysLoop;
}
