#ifndef LIBLED_H
#define LIBLED_H

#define COMMON_ANODE 1
#define COMMON_CATHODE 0

#include <cstdint>
#include "PCA9685.h"

class RGB_12bit
{
public:
    RGB_12bit(){};
    RGB_12bit(int r, int g, int b);
    RGB_12bit(int r, int g, int b, int base);
    void set(int r, int g, int b);
    int16_t R = 0;
    int16_t G = 0;
    int16_t B = 0;
    RGB_12bit operator+ (RGB_12bit const &obj);
    RGB_12bit operator- (RGB_12bit const &obj);
    int16_t& operator[] (uint8_t i);
    RGB_12bit operator/ (int factor);
    bool operator== (RGB_12bit const &obj);
};

class chromled
{
public:
    chromled(PCA9685 *chip, int R_c, int G_c, int B_c, bool eType);
    void set_color(RGB_12bit target);
    void reset();
    void linear_gradient(int steps, RGB_12bit target);
    void dimm(float scale);
    
    RGB_12bit get_color();
private:
    RGB_12bit color;
    PCA9685 *const chip;
    bool eType;
    int R_channel;
    int G_channel;
    int B_channel;
};

#endif