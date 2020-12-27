# A Library of chroma effects for Raspberry Pi
I know "RGB effect" has been such a cliché, but "chroma effect" may still have its chance (Razer somehow did a good job in it). 
With this library, you can customize chroma effects like `Rainbow`, `Wave` or so on a Raspberry Pi, and use them to build your own vibe.

But of course, with the help of a sweat tiny Pulse-width Modulation (PWM) board [PCA9685](http://github.com/Ziang95/Chroma_Pi/tree/main/data_sheets/PCA9685.pdf), 
which is very popular in controlling LEDs. A PCA9685 uses an I2C connection protocol, and can be powered by either 3.3V or 5~6V (external) source, with 
Up to 16 PWM channels. Since an RGB LED is typically 4-pin, a single PCA9685 can support up to 5 LEDs at a time -- dont't worry, 
it is way enough for a small range decoration, and after all, you can stack up PCA9685 boards to support extra LEDs.

## How to use this library?
To use this library, simply include "chroma.h" and initialize the PCA9685 before use (Need to turn on I2C port in system control of Raspberry Pi), some examples 
can be found in [examples](http://github.com/Ziang95/Chroma_Pi/tree/main/example), RGB channels of LEDs' pin heads must be specified when constructing objects of `chromled`. 
The library provides general access to each LED, but due to the nature of serial I/O, they must be serialized.<br><br>
Have fun~ :wink:
