#ifndef CHROMA_H
#define CHROMA_H

#include <vector>
#include "libled.h"

#define RED     (RGB_12bit(255, 0  , 0  , 0xFF))
#define ORANGE  (RGB_12bit(192, 63 , 0  , 0xFF))
#define YELLOW  (RGB_12bit(128, 127, 0  , 0xFF))
#define GREEN   (RGB_12bit(0  , 255, 0  , 0xFF))
#define BLUE    (RGB_12bit(0  , 0  , 255, 0xFF))
#define VIOLET  (RGB_12bit(128, 0  , 128, 0xFF))

using namespace std;

inline void set_colors(vector<chromled*> &leds, vector<RGB_12bit> &targets);
inline void set_color(vector<chromled*> &leds, RGB_12bit target);
void extinct(vector<chromled*> &leds);
void linear_gradient(vector<chromled*> &leds, vector<RGB_12bit> &targets, int steps);
void dimm_color(chromled* led, RGB_12bit color, int level);

void color_rally(vector<chromled*> &leds, vector<RGB_12bit> &color_set, int steps);
void shuttle(vector<chromled*> &leds, RGB_12bit color, int steps, int usleepT);
void shuttle(vector<chromled*> &leds, vector<RGB_12bit> &color, int steps, int usleepT);

void rainbow(vector<chromled*> &leds, int steps);
void rainbow_shuttle(vector<chromled*> &leds, int steps, int usleepT);
void blink_on(vector<chromled*> &leds, int openT, int closeT);
void blink_off(vector<chromled*> &leds, int closeT, int openT);

#endif