#ifndef CHROMA_H
#define CHROMA_H

#include <vector>
#include "libled.h"

using namespace std;

inline void chroma_set(vector<chromled*> &leds, vector<RGB_12bit> &targets);
void linear_gradient(vector<chromled*> &leds, vector<RGB_12bit> &targets, int steps);

void rainbow(vector<chromled*> &leds, int steps);

#endif