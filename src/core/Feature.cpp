/**
 * Abstract Feature class implementation
 * @author error23
 */
#include "core/Feature.hpp"

namespace CrowOs {
	namespace Core {

		/**
		 * Indicate if feature is always looping dispite of actual state
		 *
		 * @return true if feature should always loop
		 */
		boolean Feature::isAlwaysLoop() const {
			return alwaysLoop;
		}
	} // namespace Core
} // namespace CrowOs
