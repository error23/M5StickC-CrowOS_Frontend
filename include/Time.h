#ifndef TIME_H
#define TIME_H

// Includes
#include "M5StickC.h"

// Class
class Time {

private:
	RTC_TimeTypeDef upTime;
	RTC_TimeTypeDef lastActiveTime;
	int sleepTime;

	int convertTimeInSecondes(const RTC_TimeTypeDef time);
	void updateLastActiveTime(const RTC_TimeTypeDef newTime);

public:
	Time();
	void keepWokedUp();
	boolean shouldSleep();
	RTC_TimeTypeDef getUpTime();
	int getSleepTime();
	void setSleepTime(const int time);
};

#endif
