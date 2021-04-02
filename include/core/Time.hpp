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
			RTC_TimeTypeDef upTime;
			RTC_TimeTypeDef lastActiveTime;
			int sleepTime;

			int convertTimeInSecondes(const RTC_TimeTypeDef time) const;
			void updateLastActiveTime(const RTC_TimeTypeDef newTime);

		public:
			Time();
			void keepWokedUp();
			boolean shouldSleep() const;
			RTC_TimeTypeDef getUpTime() const;
			int getSleepTime() const;
			void setSleepTime(const int time);
		};

	} // namespace Core
} // namespace CrowOs
#endif
