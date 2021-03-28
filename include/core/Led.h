#ifndef LED_H
#define LED_H

// Includes
#include "M5StickC.h"

// Class
class Led {
private:
	const int LED_PIN;
	bool state;
	void setLed(const bool newState);

public:
	Led();
	void on();
	void off();
	void toggle();
};

#endif
