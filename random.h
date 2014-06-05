/*
	2048 for Arduino
	Random number generation
	
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

#ifndef RANDOM_H
#define RANDOM_H

#include <Arduino.h>
#include "config.h"

// == Function prototypes ==
/* getRandomTileValue
   
   Get the value for a new tile.
   
   Returns 2 (90% chance) or 4 (10% chance)
*/
unsigned int getRandomTileValue();

/* getRandomCoord
   
   Get a random coordinate. The generated coordinate might be already taken.
   
   Sets x and y to a coordinate of a random tile.
*/
void getRandomCoord( unsigned char *x, unsigned char *y );

/* getRandomSeed
   
   Get a random seed.
   
   Forked from http://www.utopiamechanicus.com/article/better-arduino-random-numbers/
*/
unsigned long getRandomSeed();

// == Actual code ==
/* getRandomTileValue
   
   Get the value for a new tile.
   
   Returns 2 (90% chance) or 4 (10% chance)
*/
unsigned int getRandomTileValue() {
	randomSeed( getRandomSeed() );
	unsigned int tile = random( 10 ) < 9 ? 2 : 4;
	return tile;
}

/* getRandomCoord
   
   Get a random coordinate. The generated coordinate might be already taken.
   
   Sets x and y to a coordinate of a random tile.
*/
void getRandomCoord( unsigned char *x, unsigned char *y ) {
	randomSeed( getRandomSeed() );
	(*x) = random( 4 ); // 0 to 3
	(*y) = random( 4 );
}

/* getRandomSeed
   
   Get a random seed.
   
   Forked from http://www.utopiamechanicus.com/article/better-arduino-random-numbers/
*/
unsigned long getRandomSeed() {
	unsigned long seed;
	unsigned char bits = randomBits, debias = randomDebiasLimit, i, bit;
	while ( bits-- ) {
		for ( i = 1; i <= debias; i++ )
			if ( ( bit = analogRead( pinAnalogRandomSource ) & 1 ) != ( analogRead( pinAnalogRandomSource ) & 1 ) )
				break;
		seed = ( seed << 1 ) | bit;
	}
	return seed;
}

#endif
