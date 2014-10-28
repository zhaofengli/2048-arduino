# 2048 for Arduino [![Build Status](https://travis-ci.org/zhaofengli/2048-arduino.svg?branch=master)](https://travis-ci.org/zhaofengli/2048-arduino)
A clone of the popular tile-joining game [2048](https://gabrielecirulli.github.io/2048/) for the Arduino platform.
A PCD8544 LCD screen (The one used on the "indestructible" Nokia 3110 and 5110) is used as 
its display, and four buttons are used for controlling.
After achieving 2048, the player can go on for even larger tiles (up to 32768).
The program fits inside an ATmega8, when serial interface is disabled.

A picture of it in action:
![Demo](http://zhaofengli.github.io/2048-arduino/images/demo.jpg)

## Getting started
### Preparation
1. Download the source code by [cloning the repo](https://github.com/zhaofengli/2048-arduino.git), or 
   downloading [the zipped version](https://github.com/zhaofengli/2048-arduino/archive/master.zip).
2. Edit the configuations in `config.h`.
3. If you have `enableBounceLibrary` set to 1, download and install the Bounce library from 
   [Bounce-Arduino-Wiring](https://github.com/thomasfredericks/Bounce-Arduino-Wiring).

### Compiling
Building with [Arduino-Makefile](https://github.com/sudar/Arduino-Makefile) is recommended, 
since the enclosed Makefile contains the `-mcall-prologues` flag which can save approximately 
200 bytes of flash. To build with the Arduino IDE, you may need to rename the directory to 
`_2048` and `2048.ino` to `_2048.ino`, or the IDE may complain about the file naming.

To build with the Makefile, open `Makefile` with your favourite text editor and edit the 
parameters in the file. Finally, execute the `make` command and the resulting .hex file 
will be placed under a directory named after your board tag, in the `build` directory. You 
can also use `make upload` or `make ispload` to upload the firmware to your Arduino/AVR directly.

### Hardware setup
1. Connect the PCD8544 LCD screen according to your configuations in `config.h`.
2. Connect the buttons to the coressponding pins, with the other end *grounded*.

### Play!
If you got everything right, the game is now playable. Move the numbers in four directions with 
the buttons and watch your score go up! To enable backlight, hold the right button when powering 
up. Tiles bigger than 512 are displayed as following: 1024 = "a24", 2048 = "b48", 4096 = "c96", 
8192 = "d92", 16384 = "e84", 32768 = "f68".

To adjust the contrast level (set `lcdContrastAdjustable` to 1), enter the debug mode by 
holding the down button when powering on, and press the left button to enter the adjustment mode. Press left 
and right to adjust the contrast, up to reset the contrast to the default level (`lcdDefaultContrast`). 
Note that if you have `enableDebugMode` set to 0, holding the down button when powering on will take 
you directly to the adjustment mode.

If you have `enableGameSave` set to 1, the game is automatically saved every time you reach 
a larger tile above 256 (adjustable in `saveThreshold`). To restore your game, enter the debug mode and press right.

## Problems
### The LCD screen is completely blank.
Please double check you connections first. Leave the GND pin unconnected, or connect it to ground 
via a 10uF cap. It's also possible because you have a contrast too low to display anything set. Try 
setting a higher value for `lcdDefaultContrast` in `config.h`. Some cheap displays bought online have a bad contact, 
try pressing hard on the upper frame. If you are using a 40-pin ATmega chip, you need to have JTAG disabled 
for pins in Port C to work. See the AVR datasheet for details.

### The LCD screen only displays some dots.
A dot is displayed every time the initialization enters a new stage. Stuck only with some dots 
displayed means the program is stuck when starting up. The dots mean:
* One dot: I/O directions set, backlight enabled.
* Two dots: Debouncers initialized.
* Three dots: The board is initialized and two random tiles have been placed.

### The battery meter is missing/incorrect.
The battery meter is not displayed if the measured voltage is lower than the lowest voltage of 
the battery (defined in `batteryLowestVoltage`), or the meter is disabled (`enableBatteryMeter`).
Please check your configuations in the `ADC` and `Battery meter` sections.

### The game is slow on my custom board.
The program runs best at 8MHz and above. If you have a external crystal attached, check if you 
have the correct fuse bits set.

## PC demo
You can play the game on your PC! There is `demo.cpp` demistrating the use of the platform-independent code in the project. Just build it with `g++ -o demo demo.cpp`. Use "wasd" to move the numbers.

## Contributing
You are welcome to make changes to the source code! Simply fork the repo, make your changes and 
submit a pull request. Thank you for your contributions!

## Exploring the source
* The main .ino contains only the main routines
* `base.h` contains the core logics
* `config.h` contains the configuations
* `board.h` contains a platform-independent implementation of the tile moving 
  logic
* `random.h` contains the random number generation routines
* `format.h` contains platform-independent string formatting routines
* `gamesave.h` contains game saving routines
* `battery.h` contains the battery meter code
* `display.h` contains an edited version of the excellent 
  PCD8544 driver from the Arduino wiki 
  http://playground.arduino.cc/Code/PCD8544
* `serial.h` contains a wrapper for the standard Serial library
* `bitmap.h` contains the logo bitmap
* `button.h` contains the button handling routines
* `demo.cpp` contains a quick-and-dirty wrapper of `board.h`, intended to be run on your PC
* `tests/` contains unit tests for `board.h` and `format.h`

## Acknowledgement
The project is not possible without:
* [open-source 2048 game](https://github.com/gabrielecirulli/2048) by [Gabriele Cirulli](http://www.gabrielecirulli.com)
* [Bounce library](https://github.com/thomasfredericks/Bounce-Arduino-Wiring) by [Thomas Fredericks](https://github.com/thomasfredericks)
* [PCD8544 driver](http://playground.arduino.cc/Code/PCD8544) by kuk and Sylvain Bissonnette
* Random number generation code by [David Pankhurst](http://www.utopiamechanicus.com)
* Arduino libraries by [the Arduino project](http://arduino.cc/)

## Licensing
The source code is licesed under the BSD 2-Clause License. See `LICENSE` for details.

