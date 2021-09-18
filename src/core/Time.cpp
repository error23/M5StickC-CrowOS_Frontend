/**
 * Time class implementation
 * @author error23
 */
#include "core/Time.hpp"

namespace CrowOs {
	namespace Core {

		/**
		 * Initialises new Time
		 *
		 * @param speepTime time to put device on sleep default 60, 0 for never
		 * @param maxFps max fps default 25
		 */
		Time::Time(const int sleepTime /* = 60 */, const double maxFps /* = 25 */)
			: sleepTime(sleepTime)
			, maxFps(maxFps)
			, limitFpsLastTime(0)
			, fpsLastTime(0)
			, frames(0) {

			upTime.Hours = 0;
			upTime.Minutes = 0;
			upTime.Seconds = 0;
			updateLastActiveTime(upTime);

			if(LOG_INFO) Serial.printf("Info : [Time] created with sleepTime = %d, maxFps = %fl\n", sleepTime, maxFps);
		}

		/**
		 * Set up Time helper
		 */
		void Time::setUp() {

			if(LOG_INFO) Serial.println("Info : [Time] Setup ...");
			M5.Rtc.SetTime(&upTime);
			updateLastActiveTime(upTime);
			if(LOG_INFO) Serial.println("Info : [Time] Setup Done");
		}

		/**
		 * Keep device woked up
		 *
		 * Call this method every sleep time in order to keep device woked up
		 */
		void Time::keepWokedUp() {

			if(LOG_DEBUG) Serial.println("Debug : [Time] keepWokedUp");
			updateLastActiveTime(upTime);
		}

		/**
		 * Indicates if device should fall a sleep
		 *
		 * @return true if device should fall a sleep
		 */
		boolean Time::shouldSleep() const {

			if(LOG_DEBUG) Serial.println("Debug : [Time] shouldSleep");
			if(sleepTime == 0) return false;
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

			if(LOG_DEBUG) Serial.printf("Debug : [Time] setSleepTime with time = %ds\n", time);
			sleepTime = time;
		}

		/**
		 * Limit fps
		 */
		void Time::limitFps() {

			frames++;
			M5.Rtc.GetTime(&upTime);
			unsigned long now = micros() / 1000;
			double maxFrameInterval = 1000 / (maxFps * 0.5); // calculate max time for one frame to execute

			double limitFpsElipsed = now - limitFpsLastTime; // calculate elipsed since last method call
			limitFpsLastTime = now; // update last method call
			double limitFpsDelta = floor((maxFrameInterval - limitFpsElipsed) * 0.5); // calculate delta between maxFrameInterval and last method call

			if(limitFpsDelta > 0) {
				delay(limitFpsDelta);
				if(LOG_DEBUG) Serial.printf("Debug : [Time] limitFps sleeped for %flms\n", limitFpsDelta);
			}
			else if(limitFpsDelta < 0) {
				if(LOG_DEBUG) Serial.printf("Debug : [Time] limitFps lost frames = %fl\n", -limitFpsDelta / maxFrameInterval);
			}

			double fpsElipsed = now - fpsLastTime;
			if(fpsElipsed >= 5000) {

				if(LOG_INFO) Serial.printf("Info : [Time] limitFps fps = %fl\n", frames / 5);
				fpsLastTime = now;
				frames = 0;
			}
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

				Serial.printf("Debug : [Time] updateLastActiveTime from lastActiveTime = %s with newTime = %s\n", lastActiveTimeString, timeString);
			}

			lastActiveTime.Hours = newTime.Hours;
			lastActiveTime.Minutes = newTime.Minutes;
			lastActiveTime.Seconds = newTime.Seconds;
		}
	} // namespace Core
} // namespace CrowOs
