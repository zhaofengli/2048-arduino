/*
	2048 for Arduino
	Your favourite tile-joining game on Arduino
	
	Copyright (c) 2014, Zhaofeng Li
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice, this
	  list of conditions and the following disclaimer.

	* Redistributions in binary form must reproduce the above copyright notice,
	  this list of conditions and the following disclaimer in the documentation
	  and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
	== Getting started ==
	# Edit the configuations in "config.h"
	# Connect the PCD8544 LCD screen according to
	  the configuation.
	# Buttons should be connected to GND, as the button
	  pins have pull-up enabled and therefore are "active low".
	# It's recommended to build with Arduino-Makefile, since the added
	  -mcall-prologues flag can save about 200 bytes of flash. Edit the 
	  Makefile and complie with "make".
	# If you use the Arduino IDE, the IDE may complain about 
	  the file naming. Just click "Okay" to rename the files 
	  as the IDE wants.
	# If you have enabled the Bounce library, download and install it from 
	  https://github.com/thomasfredericks/Bounce-Arduino-Wiring
	# Burn the firmware to your Arduino/AVR (an ATmega8 is enough).
	# Power up and play! Go for 2048! Of course, you can keep
	  going after reaching 2048 (up to 32768). The program displays
	  big tiles like this:
	  1024 = "a24", 2048 = "b48", 4096 = "c96", 8192 = "d92",
	  16384 = "e84", 32768 = "f68"
	# Hold the right key when powering on to activate backlight, the down 
	  key to enter debug mode (set "enableDebugMode" in config.h).
	
	== Exploring the source ==
	* The main .ino contains only the main routines
	* "base.h" contains all the core code
	* "config.h" contains the configuations
	* "pcd8544.h" contains an edited version of the excellent
	  PCD8544 driver from the Arduino wiki
	  http://playground.arduino.cc/Code/PCD8544
	* "serial.h" contains a wrapper for the standard Serial library
	* "bitmap.h" contains the logo bitmap
	* "button.h" contains the button handling routines
	  
	== Fun facts ==
	* The biggest tile you can get is 32768 (displayed as "f68"). When
	  you join two 32768 tiles (which is supposed to generate 65536 which
	  is off the range of an unsigned int), the CPU freezes.
	  
	== Acknowledgements ==
	* open-source 2048 game by Gabriele Cirulli
	* original 1024 game by Veewo Studio
	* Bounce library by Thomas Fredericks
	* PCD8544 driver by kuk and Sylvain Bissonnette
	* Random number generation code by David Pankhurst
	* Arduino libraries by the Arduino project
	
	2014 Zhaofeng Li
	~~~~
*/


// == Includes ==
// Arduino library
#include <Arduino.h>

// Configuations
#include "config.h"

// Core logics
#include "base.h"

// == Main routines ==
// Get the show on!
void setup() {
	initGame();
}

// Keep everything going!
void loop() {
	unsigned int largest, l; // largest tile
	if ( serialControl() || buttonControl() ) { // action taken
		placeNewTile();
		printBoard();
		printMessage();
		if ( ( l = getLargestTile() ) > largest ) {
			largest = l;
			if ( largest >= 512 )
				saveGame();
		}
	}
}
