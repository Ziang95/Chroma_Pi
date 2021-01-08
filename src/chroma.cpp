#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include "../headers/chroma.h"

using namespace std;

chroma_layer::chroma_layer(vector<chromled> &_leds)
{
    leds = _leds;
    layer_thread = nullptr;
}

chroma_layer::~chroma_layer()
{
    wait_thread();
}

void chroma_layer::rearrange(vector<chromled> &_leds)
{
    wait_thread();
    leds = _leds;
}

void chroma_layer::wait_thread()
{
    if (layer_thread && layer_thread->joinable())
    {
        layer_thread->join();
        delete layer_thread;
        layer_thread = nullptr;
    }
}

inline void set_colors(vector<chromled> &leds, vector<RGB_12bit> &targets)
{
    for (uint i = 0; i < leds.size(); i++)
        leds[i].set_color(targets[i]);
}

inline void set_color(vector<chromled> &leds, RGB_12bit const &color)
{
    for (uint i = 0; i < leds.size(); i++)
        leds[i].set_color(color);
}

void extinct(vector<chromled> &leds)
{
    set_color(leds, RGB_12bit(0, 0, 0));
}

void dimm_color(chromled &led, RGB_12bit color, int level)
{
    if (level > 100) level = 100;
    else if (level < 0) level = 0;
    color = color * level;
    color = color / 100;
    led.set_color(color);
}

void linear_gradient(vector<chromled> &leds, vector<RGB_12bit> targets, int steps)
{
    if (leds.size() != targets.size())
    {
        throw runtime_error("leds size doesn't match target number in linear_gradient()!");
    }

    vector<RGB_12bit> colors_tmp, distances, strides;
    bool finished = false;
    for (auto const &led : leds)
        colors_tmp.push_back(led.get_color());
    for (uint i = 0; i < targets.size(); i++)
    {
        distances.push_back(targets[i] - colors_tmp[i]);
        strides.push_back(distances[i] / steps);
        for (int j = 0; j < 3; j++)
            if (strides[i][j] == 0 && distances[i][j] != 0) strides[i][j] = distances[i][j] > 0? 1 : -1;
    }

    uint k, l;
    while (!finished)
    {
        finished = true;
        for (k = 0; k < leds.size(); k++)
        {
            if (!(colors_tmp[k] == targets[k])) finished = false;
            for (l = 0; l < 3; l++)
            {
                if (abs(colors_tmp[k][l] - targets[k][l]) <= abs(strides[k][l]))
                    colors_tmp[k][l] = targets[k][l];
                else
                    colors_tmp[k][l] += strides[k][l];
            }
            leds[k].set_color(colors_tmp[k]);
        } 
    }
}

void filtration(vector<chromled> &leds, bool (*stopSign)(void), RGB_12bit target, int steps)
{
    for (uint i = 0; i < leds.size(); i++)
    {   
        leds[i].linear_gradient(steps, target);
        if (stopSign())
            break;
    }
}

void color_rally(vector<chromled> &leds, bool (*stopSign)(), vector<RGB_12bit> color_set, int steps)
{
    vector<RGB_12bit> color_tmp(leds.size());
    int c_size = color_set.size();
    uint i = 0;
    while(1)
    {
        for (uint j = 0; j < leds.size(); j++)
            color_tmp[j] = color_set[(i + j) % c_size];
        linear_gradient(leds, color_tmp, steps);
        i = (i + 1) % c_size;
        if (stopSign())
            break;
    }
}

void rainbow(vector<chromled> &leds, bool (*stopSign)(), int steps)
{
    vector<RGB_12bit> rainbow_set{RED, ORANGE, YELLOW, GREEN, BLUE, VIOLET};
    color_rally(leds, stopSign, rainbow_set, steps);
}

void shuttle(vector<chromled> &leds, bool (*stopSign)(void), RGB_12bit color, int steps, int usleepT)
{
    int l_size = leds.size();
    if (l_size < 1) return;
    int stride = 100 / steps;
    if (stride == 0) stride = 1;
    int i = 0, j, k;
    while(1)
    {
        j = (i + 1) % l_size;
        for (k = 0; ; k += stride)
        {
            dimm_color(leds[i], color, 100 - k);
            dimm_color(leds[j], color, k);
            usleep(usleepT);
            if (k >= 100) break;
        }
        i = j;
        if (stopSign())
            break;
    }
}

void colorful_shuttle(vector<chromled> &leds, bool (*stopSign)(void), vector<RGB_12bit> color_set, int steps, int usleepT)
{
    int l_size = leds.size();
    if (l_size < 1) return;
    int c_size = color_set.size();
    int stride = 100 / steps;
    if (stride == 0) stride = 1;
    int i = 0, j, k = 0, l, m;
    while(1)
    {
        j = (i + 1) % l_size;
        l = (k + 1) % c_size;
        for (m = 0; ; m += stride)
        {
            dimm_color(leds[i], color_set[k], 100 - m);
            dimm_color(leds[j], color_set[l], m);
            usleep(usleepT);
            if (m >= 100) break;
        }
        i = j;
        k = l;
        if (stopSign())
            break;
    }
}

void rainbow_shuttle(vector<chromled> &leds, bool (*stopSign)(void), int steps, int usleepT)
{
    vector<RGB_12bit> rainbow_set{RED, ORANGE, YELLOW, GREEN, BLUE, VIOLET};
    colorful_shuttle(leds, stopSign, rainbow_set, steps, usleepT);
}

void blink_on(vector<chromled> &leds, int onT, int offT)
{
    RGB_12bit color;
    srand(time(NULL));
    color.random();
    int blinkTime = rand() % 5;

    if (rand() % 2)
    {
        set_color(leds, color);
        usleep(onT * 1000);
        extinct(leds);
        usleep(offT * 1000);
    }
    
    for (int i = 0; i < blinkTime; i++)
    {
        set_color(leds, color);
        usleep(25 * 1000);
        extinct(leds);
        usleep(25 * 1000);
    }
    set_color(leds, color);
}

void blink_off(vector<chromled> &leds, int offT, int onT)
{
    vector<RGB_12bit> colors;
    for (uint i = 0; i < leds.size(); i++)
        colors.push_back(leds[i].get_color());
    srand(time(NULL));
    int blinkTime = rand() % 5;

    for (int i = 0; i < blinkTime; i++)
    {
        extinct(leds);
        usleep(20 * 1000);
        set_colors(leds, colors);
        usleep(20 * 1000);
    }

    if (rand() % 2)
    {
        extinct(leds);
        usleep(offT * 1000);
        set_colors(leds, colors);
        usleep(onT * 1000);
    }
    extinct(leds);
}