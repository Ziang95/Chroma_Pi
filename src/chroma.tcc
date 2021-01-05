#include "../headers/chroma.h"

template<typename Callable, typename... Args>
void chroma_layer::load(Callable &&func, Args&&... args)
{
    wait_thread();
    layer_thread = new thread(func, leds, args...);
}