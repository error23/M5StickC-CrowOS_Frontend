#ifndef TIME_H
#define TIME_H

// Lib includes
#include "M5StickC.h"

// local Includes
#include "Defines.hpp"

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

			/** Max fps */
			double maxFps;

			/** Last time that limitFps was Executed */
			unsigned long limitFpsLastTime;

			/** Last time we calculated fps */
			unsigned long fpsLastTime;

			/** Number of fps since fpsLastTime */
			double frames;

			/**
			 * Updates last active time
			 *
			 * @param newTime to update
			 */
			void updateLastActiveTime(const RTC_TimeTypeDef newTime);

		public:
			/**
			 * Initialises new Time
			 *
			 * @param speepTime time to put device on sleep default 60
			 * @param maxFps max fps default 25
			 */
			Time(const int sleepTime = 60, const double maxFps = 25);

			/**
			 * Set up Time helper
			 */
			void setUp();

			/**
			 * Keep device woked up
			 *
			 * Call this method every sleep time in order to keep device woked up
			 */
			void keepWokedUp();

			/**
			 * Indicates if device should fall a sleep
			 *
			 * @return true if device should fall a sleep
			 */
			boolean shouldSleep() const;

			/**
			 * Converts RTC_TimeTypeDef to seconds
			 *
			 * @param  time to convert
			 * @return converted time
			 */
			int convertTimeToSecondes(const RTC_TimeTypeDef time) const;

			/**
			 * Converts time to string HH:MM.SS
			 *
			 * @param time to convert
			 * @param dst char* to store into has to have at least size of 9
			 */
			void convertTimeToString(const RTC_TimeTypeDef time, char dest[9]) const;

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
			 * @param time device sleep time
			 */
			void setSleepTime(const int time);

			/**
			 * Limit fps
			 */
			void limitFps();
		};

	} // namespace Core
} // namespace CrowOs
#endif
