/**
 * Time class implementation
 * @author error23
 */
#include "core/Time.hpp"

namespace CrowOs {
	namespace Core {

		/**
		 * Initialises new Time
		 */
		Time::Time() {

			upTime.Hours = 0;
			upTime.Minutes = 0;
			upTime.Seconds = 0;
			M5.Rtc.SetTime(&upTime);
			updateLastActiveTime(upTime);
		}

		/**
		 * Set up Time helper
		 */
		void Time::setUp() {
			M5.Rtc.SetTime(&upTime);
			updateLastActiveTime(upTime);
		}

		/**
		 * Keep device woked up
		 *
		 * Call this method every sleep time in order to keep device woked up
		 */
		void Time::keepWokedUp() {

			updateLastActiveTime(upTime);
		}

		/**
		 * Indicates if device should fall a sleep
		 *
		 * @return true if device should fall a sleep
		 */
		boolean Time::shouldSleep() const {

			return convertTimeInSecondes(upTime) - convertTimeInSecondes(lastActiveTime) > sleepTime;
		}

		/**
		 * Gets time since device is on
		 *
		 * @return devices up time
		 */
		RTC_TimeTypeDef Time::getUpTime() const {

			return upTime;
		}

		/**
		 * Gets device default sleep time
		 *
		 * @return devices sleep time
		 */
		int Time::getSleepTime() const {

			return sleepTime;
		}

		/**
		 * Sets device default sleep time
		 *
		 * @param time device sleep time
		 */
		void Time::setSleepTime(const int time) {

			sleepTime = time;
		}

		/**
		 * Converts RTC_TimeTypeDef in seconds
		 *
		 * @param  time to convert
		 * @return converted time
		 */
		int Time::convertTimeInSecondes(const RTC_TimeTypeDef time) const {

			return time.Hours * 3600 + time.Minutes * 60 + time.Seconds;
		}

		/**
		 * Updates last active time
		 *
		 * @param newTime to update
		 */
		void Time::updateLastActiveTime(const RTC_TimeTypeDef newTime) {

			lastActiveTime.Hours = newTime.Hours;
			lastActiveTime.Minutes = newTime.Minutes;
			lastActiveTime.Seconds = newTime.Seconds;
		}
	} // namespace Core
} // namespace CrowOs
