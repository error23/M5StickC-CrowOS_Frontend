#ifndef TIME_H
#define TIME_H

// Lib includes
#include "M5StickC.h"

namespace CrowOs {
	namespace Core {

		/**
		 * Time class
		 *
		 * Used to manage device upTime and sleepTime
		 */
		class Time {

		private:
			/** Time since device is up */
			RTC_TimeTypeDef upTime;

			/** Time since last keepWokedUp call */
			RTC_TimeTypeDef lastActiveTime;

			/** Default time to put device on sleep */
			int sleepTime;

			/**
			 * Converts RTC_TimeTypeDef in seconds
			 *
			 * @param  time                  to convert
			 * @return                       converted time
			 */
			int convertTimeInSecondes(const RTC_TimeTypeDef time) const;

			/**
			 * Updates last active time
			 *
			 * @param  newTime              to update
			 */
			void updateLastActiveTime(const RTC_TimeTypeDef newTime);

		public:
			/**
			 * Initialises new Time
			 */
			Time();

			/**
			 * Keep device woked up
			 *
			 * Call this method every sleep time in order to keep device woked up
			 */
			void keepWokedUp();

			/**
			 * Indicates if device should fall a sleep
			 *
			 * @return  true if device shoudl fall a sleep
			 */
			boolean shouldSleep() const;

			/**
			 * Gets time since device is on
			 *
			 * @return devices up time
			 */
			RTC_TimeTypeDef getUpTime() const;

			/**
			 * Gets device default sleep time
			 *
			 * @return devices sleep time
			 */
			int getSleepTime() const;

			/**
			 * Sets device default sleep time
			 *
			 * @param  time         device sleep time
			 */
			void setSleepTime(const int time);
		};

	} // namespace Core
} // namespace CrowOs
#endif
