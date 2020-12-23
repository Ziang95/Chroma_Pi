#include <iostream>
#include "../headers/chroma.h"

using namespace std;

inline void chroma_set(vector<chromled*> &leds, vector<RGB_12bit> &targets)
{
    for (int i = 0; i < leds.size(); i++)
    {
        leds[i]->set_color(targets[i]);
    }
}

void linear_gradient(vector<chromled*> &leds, vector<RGB_12bit> &targets, int steps)
{
    if (leds.size() != targets.size())
    {
        cout << "led counts and target number doesn't match!" << endl;
        return;
    }

    vector<RGB_12bit> colors_tmp, distances, strides;
    bool finished = false;
    for (auto const &led : leds)
        colors_tmp.push_back(led->get_color());
    for (int i = 0; i < targets.size(); i++)
    {
        distances.push_back(targets[i] - colors_tmp[i]);
        strides.push_back(distances[i] / steps);
        for (int j = 0; j < 3; j++)
            if (strides[i][j] == 0 && distances[i][j] != 0) strides[i][j] = distances[i][j] > 0? 1 : -1;
    }

    int k, l;
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
            leds[k]->set_color(colors_tmp[k]);
        } 
    }
}

void rainbow(vector<chromled*> &leds, int steps)
{
    RGB_12bit red   (255, 0  , 0  , 0xFF);
    RGB_12bit orange(192, 63 , 0  , 0xFF);
    RGB_12bit yellow(128, 127, 0  , 0xFF);
    RGB_12bit green (0  , 255, 0  , 0xFF);
    RGB_12bit blue  (0  , 0  , 255, 0xFF);
    RGB_12bit purple(63 , 0  , 192, 0xFF);

    vector<RGB_12bit> rainbow_set{red, orange, yellow, green, blue, purple};
    vector<RGB_12bit> rainbow_tmp(leds.size());

    int r_size = rainbow_set.size();
    uint i = 0;
    while(1)
    {
        for (int k = 0; k < leds.size(); k++)
        {
            rainbow_tmp[k] = rainbow_set[(i % r_size + k) % r_size];
        }
        linear_gradient(leds, rainbow_tmp, steps);
        i = (i + 1);
    }
}