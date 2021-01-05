#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
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

    //---------OOP Programming----------
    chroma_layer layer(leds);
    layer.load(extinct);
    sleep(1);
    layer.load(blink_on, 40, 100);
    sleep(1);
    layer.load(blink_off, 100, 80);
    //----------------------------------

    //------Functional Programming------
    // extinct(leds);
    // sleep(1);
    // blink_on(leds, 40, 100);
    // sleep(1);
    // blink_off(leds, 100, 80);
    //----------------------------------
}