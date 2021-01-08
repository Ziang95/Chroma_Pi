#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "../headers/libled.h"

using namespace std;

RGB_12bit::RGB_12bit(int r, int g, int b)
{
    this->R = r;
    this->G = g;
    this->B = b;
}

RGB_12bit::RGB_12bit(int r, int g, int b, int base)
{
    this->R = r * 0xFFF / base;
    this->G = g * 0xFFF / base;
    this->B = b * 0xFFF / base;
}

void RGB_12bit::set(int r, int g, int b)
{
    this->R = r;
    this->G = g;
    this->B = b;
}

void RGB_12bit::random()
{
    for (int i = 0; i < 3; i++)
        (*this)[i] = rand() % 0xFFF;
    (*this) = (*this) * 0xFFF / (R + G + B);
}

void RGB_12bit::calibrate(int brightness, int base)
{
    int sum = R + G + B;
    int target_sum = 0xFFF * brightness / base;

    R = R * target_sum / sum;
    G = G * target_sum / sum;
    B = B * target_sum / sum;
}

RGB_12bit RGB_12bit::operator+ (RGB_12bit const &obj)
{
    RGB_12bit ret(R + obj.R, G + obj.G, B + obj.B);
    return ret;
}

RGB_12bit RGB_12bit::operator- (RGB_12bit const &obj)
{
    RGB_12bit ret(R - obj.R, G - obj.G, B - obj.B);
    return ret;
}

int& RGB_12bit::operator[] (uint8_t i)
{
    switch (i)
    {
    case 0:
        return this->R;
    case 1:
        return this->G;
    default:
        return this->B;
    }
}

RGB_12bit RGB_12bit::operator* (int mltpler)
{
    RGB_12bit ret(R * mltpler, G * mltpler, B * mltpler);
    return ret;
}

RGB_12bit RGB_12bit::operator/ (int divisor)
{
    RGB_12bit ret(R / divisor, G / divisor, B / divisor);
    return ret;
}

bool RGB_12bit::operator== (RGB_12bit const &obj)
{
    return (R == obj.R) && (G == obj.G) && (B == obj.B);
}

ostream& operator<< (ostream &os, RGB_12bit const& obj)
{
    cout << "R=" << obj.R << "\tG=" << obj.G << "\tB=" << obj.B;
    return os;
}

chromled::chromled(PCA9685 *chip, int R_c, int G_c, int B_c, bool eType)
:chip(chip)
{
    this->R_channel = R_c;
    this->G_channel = G_c;
    this->B_channel = B_c;
    this->eType = eType;
}

void chromled::set_color(RGB_12bit target)
{
    target.R = target.R > PWM_MAX_RSLN ? PWM_MAX_RSLN : target.R;
    target.G = target.G > PWM_MAX_RSLN ? PWM_MAX_RSLN : target.G;
    target.B = target.B > PWM_MAX_RSLN ? PWM_MAX_RSLN : target.B;
    if (eType == COMMON_ANODE)
    {
        chip->setPWM(R_channel, target.R, PWM_MAX_RSLN);
        chip->setPWM(G_channel, target.G, PWM_MAX_RSLN);
        chip->setPWM(B_channel, target.B, PWM_MAX_RSLN);
    }
    else
    {
        chip->setPWM(R_channel, 0, PWM_MAX_RSLN - target.R);
        chip->setPWM(G_channel, 0, PWM_MAX_RSLN - target.G);
        chip->setPWM(B_channel, 0, PWM_MAX_RSLN - target.B);
    }
    color = target;
}

void chromled::reset()
{
    RGB_12bit dark(0, 0, 0);
    set_color(dark);
}

RGB_12bit chromled::get_color() const
{
    return this->color;
}

void chromled::linear_gradient(int steps, RGB_12bit target)
{
    RGB_12bit distance = target - color;
    RGB_12bit color_tmp = color;
    RGB_12bit stride = distance / steps;

    for (int i = 0; i < 3; i++)
        if (stride[i] == 0 && distance[i] != 0)
            stride[i] = distance[i] > 0? 1 : -1;

    while (!(color_tmp == target))
    {
        for (int i = 0; i < 3; i++)
        {
            if (abs(color_tmp[i] - target[i]) < abs(distance[i]))
                color_tmp[i] = target[i];
            else
                color_tmp[i] += stride[i];
        }
        set_color(color_tmp);
    }
}