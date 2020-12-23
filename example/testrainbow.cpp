#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include "../headers/libled.h"
#include "../headers/chroma.h"

#define STEPS 512

using namespace std;

int main()
{
    PCA9685 hat(0x40, 1);
    hat.init();
    hat.setFreq(1525);
    chromled led0(&hat, 9, 10, 11, COMMON_ANODE);
    chromled led1(&hat, 13, 14, 15, COMMON_ANODE);
    vector<chromled*> leds{&led0, &led1};
    rainbow(leds, STEPS);
}