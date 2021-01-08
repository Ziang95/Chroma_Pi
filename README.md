# A Library of chroma effects for Raspberry Pi
I know "RGB effect" has been such a cliché, but "chroma effect" may still have its chance (Razer somehow did a good job on it). 
With this library, you can customize chroma effects like `Rainbow`, `Wave` or so on a Raspberry Pi, and use them to build your own vibe.

But of course, with the help of a sweet tiny Pulse-width Modulation (PWM) board [PCA9685](http://github.com/Ziang95/Chroma_Pi/tree/main/data_sheets/PCA9685.pdf), 
which is very popular in controlling LEDs. A PCA9685 uses an I2C connection protocol, and can be powered by either 3.3V or 5~6V (external) source, with 
Up to 16 PWM channels. Since an RGB LED is typically 4-pin, a single PCA9685 can support up to 5 LEDs at a time -- dont't worry, 
it is way enough for a small range decoration, and after all, you can stack up PCA9685 boards to support extra LEDs.

## How to Start?
Simply put headers/ in your project and include "chroma.h", then initialize the PCA9685 `(Need to turn on I2C port in system control of Raspberry Pi)`. Say if your PCA9685 chip is connected to bus I2C-1 and its address is 0x40, you should start with:
```c++
PCA9685 board(0x40, 1);
board.init();            //Initialize PCA9685
board.setFreq(1525);     //Set the default frequency, from 24Hz ~ 1526Hz
``` 
Then initialize chroma leds. For example, there is an LED with its R,G,B pins connected to channel 0,1,2 respectively on `board`, and the LED uses common anode:
```c++
chromled led_0(&board, 0, 1, 2, COMMON_ANODE);
chromled led_1(&board, 3, 4, 5, COMMON_CATHODE);  //Another LED connected to channel 3,4,5 and uses common cathode
```
After that, if your chroma array consists of several LEDs, construct the array:
```c++
vector<chromled> led_array({led_0, led_1, led_2, led_3, ... });
```
### Blocking or non-blocking
Now you have two options: either call chroma effect function directly or construct an object of chroma_layer. But please keep in mind that some chroma functions involve infinite loop and are therefore blocking, you need to pass a semaphore checker function as parameter to tell them when to stop:
```c++
bool time_to_stop()
{
    /*Do something*/
    return semaphore == CRITICAL;
}

rainbow(led_array, time_to_stop, 256); 
/*the '256' here is the steps of color morphing stage, higher value means smoother color change
e.g. current color.R = 0, target color.R = 512, then it divides each step into 512/256 = 2
minimal footprint is 1 */
```
But if you want the chroma function to be non-blocking and let the library to handle threading for you, try:
```c++
chroma_layer layer0(led_array);            //Use an LED array to construct an object of chroma layer
layer0.load(rainbow, time_to_stop, 256);   //Load the chroma function and all parameters into the object, 
                                           //(Note that you no longer have to pass the LED array to it)
                                           //and the light effect will start right away
```
some examples can be found in [examples](http://github.com/Ziang95/Chroma_Pi/tree/main/example)
## Let's talk about colors
Since the resolution of PCA9685's configurable duty cycle is 12-bit, the RGB profile used in this project is also 12-bit. A color profile can be constructed in many ways:
```c++
RGB_12bit color_a;                      //color_a.R = color_a.G = color_a.G = 0
RGB_12bit color_b(1024, 4095, 0);       //color_b.R = 1024, color_b.G = 4095, color_b.B = 0

/*If you want to initialize it using 8-bit color profile*/
RGB_12bit color_c(127, 127, 0, 0xFF);   //color_c.R = (127/0xFF)*0xFFF
```
And capable for arithmetic:
```c++
RGB_12bit color_d(128, 128, 1024);
RGB_12bit color_e(256, 0, 128);

RGB_12bit color_f = color_d - color_e;   //color_f.R = -128, color_f.G = 128, color_f.B = 896, note that negative value is allowed here
RGB_12bit color_g = color_d * 2;         //color_g.R = 256, color_g.G = 256, color_g.B = 2048
RGB_12bit color_h = color_e / 2;         //color_h.R = 128, color_h.G = 0, color_h.B = 64
```
## Write your own chroma effect
If you have your own idea, just create a new function and load it into the chrom_layer, for example:
```c++
void my_chroma(vector<chromled> &led_array, vector<RGB_12bit> color_set, int time)
{
    /* Do something */
}

void my_loop_chroma(vector<chromled> &led_array, bool (*stopSign)(), int steps)
{
    /* Do something */
}

chroma_layer layer1(led_array);

layer1.load(my_chroma, my_color_set, 2048);

layer1.load(my_loop_chroma, semaphore_checker, 2000);
```
Please keep in mind that a layer won't start any new chroma effect until its preceding chroma effect is finished or terminated.
