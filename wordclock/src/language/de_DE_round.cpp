#include <Arduino.h>

#include "de_DE_round.h"

#include "../config.h"
#include "../dnd.h"
#include "../led.h"

void Grid_de_DE_round::setTime(int hour, int minute) {
  if(hour == -1 || minute == -1) {
	return;
  }

  if(DND::active(hour, minute)) {
	for(int i = 0; i < NUM_LEDS; i++) {
	  Led::ids[i].setRGB(0, 0, 0);
	}

	FastLED.show();
  yield(); 
  FastLED.delay(4);

	return;
  }
  // minuteDifference will show the difference to the shown clock (-2 minutes or +2 minutes)
  // I use 5 led the center led means +0 min
  int correctMinute = minute;
  int hourLimit = 6;

  // round the minutes
  // 7:57 will show 7:55 and 7:58 will show 8:00
  minute += 2;
  if(minute >= 60) {
	  hour += 1;
    minute -= 60;
  }
  minute = (minute - (minute % 5));
  int minuteDifference = correctMinute - minute;
  // if the difference is -2 minutes - the first 3 led lights (0|0|0|x|x)
  // if the clock shows the correct time (diff = 0) the third led lights (x|x|0|x|x)
  // if the difference is +1 minute - the following led lights (x|x|0|0|x)

  if(minute >= 25) {
	hour += 1;
  }

  minute = minute / 5;
  hour = hour % 12;

  for(int i = 0; i < NUM_LEDS; i++) {
	Led::ids[i].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
  }

  for(int i = 0; i < 5; i++) {
	Led::ids[Led::getLedId(Grid_de_DE_round::time_it_is[i])].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
  }

  for(int m = 0; m < 12; m++) {
	if(Grid_de_DE_round::time_minutes[minute][m] >= 0) {
	  Led::ids[Led::getLedId(Grid_de_DE_round::time_minutes[minute][m])].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
	}
  }

  if(hour == 1 && minute == 0) {
	  hourLimit = 3; // show "ein" instead of "eins"
  }

  for(int h = 0; h < hourLimit; h++) {
    if(Grid_de_DE_round::time_hours[hour][h] >= 0) {
      Led::ids[Led::getLedId(Grid_de_DE_round::time_hours[hour][h])].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
    }
  }


  int singleMinutesFirstLED = 0;
  if(GRID_SINGLE_MINUTES == 1) {
	  singleMinutesFirstLED = NUM_LEDS - EXTRA_LEDS;
  }

  // if the difference is -2 minutes - the first 2 led lights (0|0|x|x|x)
  // if the clock shows the correct time (diff = 0) the third led lights (x|x|0|x|x)
  // if the difference is +1 minute - the following led lights (x|x|x|0|x)
  switch(minuteDifference)
  { 
    case -2:
      Led::ids[singleMinutesFirstLED].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
      Led::ids[singleMinutesFirstLED + 1].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
      break;
    case -1:
      Led::ids[singleMinutesFirstLED + 1].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
      break;
    case 0:
      Led::ids[singleMinutesFirstLED + 2].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
      break;
    case 1:
      Led::ids[singleMinutesFirstLED + 3].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
      break;
    case +2:
      Led::ids[singleMinutesFirstLED + 3].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
      Led::ids[singleMinutesFirstLED + 4].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
      break;
    default:
      break;
  }
  FastLED.setBrightness(Config::brightness * 255);
  FastLED.show();
  yield(); 
  FastLED.delay(4);
}

int Grid_de_DE_round::time_it_is[5] = {0, 1, 3, 4, 5}; // es ist

int Grid_de_DE_round::time_minutes[12][12] = {
  {107, 108, 109,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // uhr
  {  7,   8,   9,  10,  35,  36,  37,  38,  -1,  -1,  -1,  -1}, // fünf nach
  { 11,  12,  13,  14,  35,  36,  37,  38,  -1,  -1,  -1,  -1}, // zehn nach
  { 26,  27,  28,  29,  30,  31,  32,  35,  36,  37,  38,  -1}, // viertel nach
  { 15,  16,  17,  18,  19,  20,  21,  35,  36,  37,  38,  -1}, // zwanzig nach
  {  7,   8,   9,  10,  39,  40,  41,  44,  45,  46,  47,  -1}, // fünf vor halb
  { 44,  45,  46,  47,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // halb
  {  7,   8,   9,  10,  35,  36,  37,  38,  44,  45,  46,  47}, // fünf nach halb
  { 15,  16,  17,  18,  19,  20,  21,  39,  40,  41,  -1,  -1}, // zwanzig vor
  { 26,  27,  28,  29,  30,  31,  32,  39,  40,  41,  -1,  -1}, // viertel vor
  { 11,  12,  13,  14,  39,  40,  41,  -1,  -1,  -1,  -1,  -1}, // zehn vor
  {  7,   8,   9,  10,  39,  40,  41,  -1,  -1,  -1,  -1,  -1}  // fünf vor
};

int Grid_de_DE_round::time_hours[12][6] = {
  { 49,  50,  51,  52,  53,  -1}, // zwölf
  { 57,  58,  59,  60,  -1,  -1}, // eins
  { 55,  56,  57,  58,  -1,  -1}, // zwei
  { 67,  68,  69,  70,  -1,  -1}, // drei
  { 84,  85,  86,  87,  -1,  -1}, // vier
  { 73,  74,  75,  76,  -1,  -1}, // fünf
  {100, 101, 102, 103, 104,  -1}, // sechs
  { 60,  61,  62,  63,  64,  65}, // sieben
  { 89,  90,  91,  92,  -1,  -1}, // acht
  { 80,  81,  82,  83,  -1,  -1}, // neun
  { 93,  94,  95,  96,  -1,  -1}, // zehn
  { 77,  78,  79,  -1,  -1,  -1}  // elf
};
