#include "core/Time.h"

/**
 * Set up Time
 */
CrowOs::Core::Time::Time() {

	upTime.Hours = 0;
	upTime.Minutes = 0;
	upTime.Seconds = 0;
	M5.Rtc.SetTime(&upTime);
	updateLastActiveTime(upTime);
}

/**
 * Keep woked up
 */
void CrowOs::Core::Time::keepWokedUp() {

	updateLastActiveTime(upTime);
}

/**
 * Indicates if device should sleep
 *
 * @return true if device should sleep
 */
boolean CrowOs::Core::Time::shouldSleep() {

	return convertTimeInSecondes(upTime) - convertTimeInSecondes(lastActiveTime) > sleepTime;
}

/**
  * Converts RTC_TimeTypeDef in int seconds
  *
  * @param time to Convert
  * @return converted time in seconds
  */
int CrowOs::Core::Time::convertTimeInSecondes(const RTC_TimeTypeDef time) {

	return time.Hours * 3600 + time.Minutes * 60 + time.Seconds;
}

/**
 * Set last active time
 *
 * @param newTime to set
 */
void CrowOs::Core::Time::updateLastActiveTime(const RTC_TimeTypeDef newTime) {

	lastActiveTime.Hours = newTime.Hours;
	lastActiveTime.Minutes = newTime.Minutes;
	lastActiveTime.Seconds = newTime.Seconds;
}

/**
 * @return Time since start up
 */
RTC_TimeTypeDef CrowOs::Core::Time::getUpTime() {

	return upTime;
}

/**
 * @return sleepTime
 */
int CrowOs::Core::Time::getSleepTime() {

	return sleepTime;
}

/**
 * sets default sleep time
 *
 * @param time to set
 */
void CrowOs::Core::Time::setSleepTime(const int time) {

	sleepTime = time;
}
