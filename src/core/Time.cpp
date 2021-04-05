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
			if(LOG_INFO) Serial.println("Info : Time created");
		}

		/**
		 * Set up Time helper
		 */
		void Time::setUp() {

			M5.Rtc.SetTime(&upTime);
			updateLastActiveTime(upTime);
			if(LOG_INFO) Serial.println("Info : Time Setup Done");
		}

		/**
		 * Keep device woked up
		 *
		 * Call this method every sleep time in order to keep device woked up
		 */
		void Time::keepWokedUp() {

			if(LOG_DEBUG) Serial.println("Debug : Time keepWokedUp");
			updateLastActiveTime(upTime);
		}

		/**
		 * Indicates if device should fall a sleep
		 *
		 * @return true if device should fall a sleep
		 */
		boolean Time::shouldSleep() const {

			if(LOG_DEBUG) Serial.println("Debug : Time shouldSleep");
			return convertTimeToSecondes(upTime) - convertTimeToSecondes(lastActiveTime) > sleepTime;
		}

		/**
		 * Converts RTC_TimeTypeDef to seconds
		 *
		 * @param  time to convert
		 * @return converted time
		 */
		int Time::convertTimeToSecondes(const RTC_TimeTypeDef time) const {

			return time.Hours * 3600 + time.Minutes * 60 + time.Seconds;
		}

		/**
		 * Converts time to string HH:MM.SS
		 *
		 * @param time to convert
		 * @param dst char* to store into has to have at least size of 9
		 */
		void Time::convertTimeToString(const RTC_TimeTypeDef time, char dest[9]) const {

			snprintf(dest, 9, "%d:%d:%d", time.Hours, time.Minutes, time.Seconds);
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

			if(LOG_DEBUG) Serial.printf("Debug : Time setSleepTime with time = %ds\n", time);
			sleepTime = time;
		}

		/**
		 * Updates last active time
		 *
		 * @param newTime to update
		 */
		void Time::updateLastActiveTime(const RTC_TimeTypeDef newTime) {

			if(LOG_DEBUG) {

				char timeString[9];
				convertTimeToString(newTime, timeString);

				char lastActiveTimeString[9];
				convertTimeToString(lastActiveTime, lastActiveTimeString);

				Serial.printf("Debug : Time updateLastActiveTime from lastActiveTime = %s with newTime = %s\n", lastActiveTimeString, timeString);
			}

			lastActiveTime.Hours = newTime.Hours;
			lastActiveTime.Minutes = newTime.Minutes;
			lastActiveTime.Seconds = newTime.Seconds;
		}
	} // namespace Core
} // namespace CrowOs
