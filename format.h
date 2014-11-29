/*
	2048 for Arduino
	String formatting
	
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

// Keep this file platform-independent!

#ifndef FORMAT_H
#define FORMAT_H

#include <string.h>

// == Function prototypes ==
/* getTileString

   gives buffer a 3-char string (actually 4-char, including \0)
   of the tile, for printing on LCD.
   
   for tiles smaller than 1024, it right-pads the number, 
   like 8 = "8  "
   
   for bigger values, it prepends the number with a marker, followed
   by the last 2 digits of the number:
   1024 = "a24", 2048 = "b48", 4096 = "c96", 8192 = "d92",
   16384 = "e84", 32768 = "f68"
   
   P.S. 32768 is the biggest tile we can get (with an unsigned int)
*/
char * getTileString( char * buffer, unsigned int value );

/* getLineString

   feeds buffer a string for printing on LCD as a line (12 chars)
*/
char * getLineString( char * buffer, char * str );

// == Actual code ==
/* getTileString

   gives buffer a 3-char string (actually 4-char, including \0)
   of the tile, for printing on LCD.
   
   for tiles smaller than 1024, it right-pads the number, 
   like 8 = "8  "
   
   for bigger values, it prepends the number with a marker, followed
   by the last 2 digits of the number:
   1024 = "a24", 2048 = "b48", 4096 = "c96", 8192 = "d92",
   16384 = "e84", 32768 = "f68"
   
   P.S. 32768 is the biggest tile we can get (with an unsigned int)
*/
char * getTileString( char * buffer, unsigned int value ) {
	if ( value == 0 ) {
		strcpy( buffer, ".  " );
		return buffer;
	} else if ( value < 1024 ) { // just right-pad it!
		sprintf( buffer, "%-3u", value );
		return buffer;
	} else { // we need to prepend a marker
		switch ( value ) { // hard-coded :P
			case 1024:
				strcpy( buffer, "a24" );
				break;
			case 2048:
				strcpy( buffer, "b48" );
				break;
			case 4096: // Excellent!
				strcpy( buffer, "c96" );
				break;
			case 8192:// Unbelievable!
				strcpy( buffer, "d92" );
				break;
			case 16384: // Wow.
				strcpy( buffer, "e84" );
				break;
			case 32768: // You must be crazy.
				strcpy( buffer, "f68" );
				break;
			default: // for something that shouldn't happen...
				strcpy( buffer, "xxx" );
				break;
		}
		return buffer;
	}
}

/* getLineString

   feeds buffer a string for printing on LCD as a line (12 chars)
*/
char * getLineString( char * buffer, char * str ) {
	sprintf( buffer, "%-12.12s", str );
	return buffer;
}

#endif
