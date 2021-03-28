#ifndef SCREEN_H
#define SCREEN_H

// Includes
#include "M5StickC.h"

// Defines
#define BACKGROUND_COLOR TFT_BLACK

// Class
class Screen {
public:
	Screen();
	void printLCD(uint16_t color, const char* text, int x, int y);
	void clearLCD();
	void clearLCD(int size, int x, int y);
};
#endif
