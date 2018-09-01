#ifndef _MENU_H
#define _MENU_H


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


#define MENU_ROW_HEIGHT 9
#define MENU_HEADER_OFFSET 3



void menu();



#endif //_MENU_H
