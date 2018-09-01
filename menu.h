#ifndef _MENU_H
#define _MENU_H


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#ifndef SSD1306_128_64
#error("Display type incorrect, please change Adafruit_SSD1306.h to use SSD1306_128_64 (128x64) display.");
#endif


#define MENU_ROW_HEIGHT 9
#define MENU_HEADER_OFFSET 3



void menu();
void drawSensorPixels();



#endif //_MENU_H
