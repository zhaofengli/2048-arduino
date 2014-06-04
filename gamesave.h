/*
	2048 for Arduino
	Game save
	
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

// Keep this file Arduino-free!

#include <avr/eeprom.h>
#include "config.h"

// == Runtime variables ==
// Game save
#if enableGameSave
unsigned int boardsave[4][4] EEMEM;
unsigned char boardsaveflag EEMEM = 0;
#endif

// == Function prototypes ==
/* saveGame
   
   Save the board to EEPROM.
   
   Returns true if successful.
*/
bool saveGame( unsigned int board[][4] );

/* loadGame
   
   Restore the saved game from EEPROM
   
   Returns true if successful.
*/
bool loadGame( unsigned int board[][4] );

/* clearSave
   
   Clear the game save.
   
   Returns true if successful.
*/
bool clearSave();

// == Actual code ==
/* saveGame
   
   Save the board to EEPROM.
   
   Returns true if successful.
*/
bool saveGame( unsigned int board[][4] ) {
#if enableGameSave
	unsigned char x, y;
	for ( x = 0; x <= 3; x++ )
		for ( y = 0; y <= 3; y++ )
			eeprom_write_word( &boardsave[x][y], board[x][y] );
	eeprom_write_byte( &boardsaveflag, 1 );
	return true;
#else
	return false;
#endif
}

/* loadGame
   
   Restore the saved game from EEPROM
   
   Returns true if successful.
*/
bool loadGame( unsigned int board[][4] ) {
#if enableGameSave
	if ( eeprom_read_byte( &boardsaveflag ) == 1 ) {
		unsigned char x, y;
		for ( x = 0; x <= 3; x++ )
			for ( y = 0; y <= 3; y++ )
				board[x][y] = eeprom_read_word( &boardsave[x][y] );
		return true;
	} else
		return false;
#else
	return false;
#endif
}

/* clearSave
   
   Clear the game save.
   
   Returns true if successful.
*/
bool clearSave();