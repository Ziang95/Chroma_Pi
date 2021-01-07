#ifndef LIBLED_H
#define LIBLED_H

#define COMMON_ANODE (1)
#define COMMON_CATHODE (0)

#include <time.h>
#include <cstdint>
#include <iostream>
#include "PCA9685.h"

using namespace std;

class RGB_12bit
{
public:
    RGB_12bit(){};
    RGB_12bit(int r, int g, int b);
    RGB_12bit(int r, int g, int b, int base);
    int R = 0;
    int G = 0;
    int B = 0;

    void set(int r, int g, int b);
    void random();
    void calibrate(int brightness, int base);

    int& operator[] (uint8_t i);
    RGB_12bit operator+ (RGB_12bit const &obj);
    RGB_12bit operator- (RGB_12bit const &obj);
    RGB_12bit operator* (int mltpler);
    RGB_12bit operator/ (int divisor);
    friend ostream& operator<< (ostream &os, RGB_12bit const &obj);
    bool operator== (RGB_12bit const &obj);
};

class chromled
{
public:
    chromled(PCA9685 *chip, int R_c, int G_c, int B_c, bool eType);
    void set_color(RGB_12bit target);
    void reset();
    void linear_gradient(int steps, RGB_12bit target);

    RGB_12bit get_color();
private:
    RGB_12bit color = RGB_12bit(0, 0, 0);
    PCA9685 *const chip = nullptr;
    bool eType;
    int R_channel;
    int G_channel;
    int B_channel;
};

#endif