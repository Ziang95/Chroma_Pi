#include <stdlib.h>
#include <iostream>
#include <vector>
#include "../headers/libled.h"
#include "../headers/chroma.h"

using namespace std;

int main(int argc, char** argv)
{
    PCA9685 hat(0x40, 1);
    hat.init();
    hat.setFreq(1525);
    chromled led0(&hat, 0, 1, 2, COMMON_ANODE);
    chromled led1(&hat, 3, 4, 5, COMMON_ANODE);
    vector<chromled*> leds{&led0, &led1};
    
    rainbow_shuttle(leds, 100, 10000);
}