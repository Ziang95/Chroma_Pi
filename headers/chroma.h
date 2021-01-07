#ifndef CHROMA_H
#define CHROMA_H

#include <vector>
#include <thread>
#include "libled.h"

using namespace std;

#define RED     (RGB_12bit(255, 0  , 0  , 0xFF))
#define ORANGE  (RGB_12bit(192, 63 , 0  , 0xFF))
#define YELLOW  (RGB_12bit(128, 127, 0  , 0xFF))
#define GREEN   (RGB_12bit(0  , 255, 0  , 0xFF))
#define BLUE    (RGB_12bit(0  , 0  , 255, 0xFF))
#define VIOLET  (RGB_12bit(128, 0  , 128, 0xFF))

using namespace std;

class chroma_layer
{
public:
    chroma_layer(vector<chromled*> const &_leds);
    ~chroma_layer();
    template<typename Callable, typename... Args>
    void load(Callable &&func, Args&&... args);
    void rearrange(vector<chromled*> const &_leds);
private:
    void wait_thread();
    thread *layer_thread;
    vector<chromled*> leds;
};

inline void set_colors(vector<chromled*> const &leds, vector<RGB_12bit> const &targets);
inline void set_colors(vector<chromled*> const &leds, RGB_12bit const &target);
void extinct(vector<chromled*> leds);
void linear_gradient(vector<chromled*> leds, vector<RGB_12bit> targets, int steps);
void dimm_color(chromled* _led, RGB_12bit color, int level);
void blink_on(vector<chromled*> leds, int openT, int closeT);
void blink_off(vector<chromled*> leds, int closeT, int openT);

void filtration(vector<chromled*> leds, bool (*stopSign)(void), RGB_12bit target, int steps);
void color_rally(vector<chromled*> leds, bool (*stopSign)(void), vector<RGB_12bit> color_set, int steps);
void shuttle(vector<chromled*> leds, bool (*stopSign)(void), RGB_12bit color, int steps, int usleepT);
void colorful_shuttle(vector<chromled*> leds, bool (*stopSign)(void), vector<RGB_12bit> color, int steps, int usleepT);

void rainbow(vector<chromled*> leds, bool (*stopSign)(void), int steps);
void rainbow_shuttle(vector<chromled*> leds, bool (*stopSign)(void), int steps, int usleepT);


template<typename Callable, typename... Args>
void chroma_layer::load(Callable &&func, Args&&... args)
{
    wait_thread();
    layer_thread = new thread(func, leds, args...);
}

#endif