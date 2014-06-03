/*
	2048 for Arduino
	Core
	
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

// == Includes ==
#include <Arduino.h>
#include <math.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "display.h"
#include "button.h"
#include "serial.h"
#include "bitmap.h"

// == Runtime variables ==

// The board
unsigned int board[4][4]; // 0 ~ 65535

// Game save
#if enableGameSave
unsigned int boardsave[4][4] EEMEM;
unsigned char boardsaveflag EEMEM = 0;
#endif

// Current score
unsigned int score = 0;


// == Function prototypes ==

// === Tile moving ===
/* moveOnce

   moves a tile in a direction by a tile, and returns 1 if
   moved to a empty space, 2 if merged with another tile, 0 if not
   moved at all.
   
   if noMerge is true, the function refuses to join tiles.
*/
unsigned char moveOnce( unsigned char x, unsigned char y, bool h, bool v, bool hv, bool vv, bool noMerge );

/* moveVerticallyOnce, moveHorizontallyOnce
   
   Wrappers of moveOnce() for convenience.
*/
#define moveVerticallyOnce( x, y, d, noMerge ) moveOnce( x, y, false, true, false, d, noMerge )
#define moveHorizontallyOnce( x, y, d, noMerge ) moveOnce( x, y, true, false, d, false, noMerge )

/* moveUp, moveLeft, moveDown, moveRight

   move everything to a direction, returns true if
   any tile is moved.
*/
bool moveUp();
bool moveLeft();
bool moveDown();
bool moveRight();

// === Game process & Scoring ===

/* placeNewTile
   
   Place a new tile on the board.
*/
void placeNewTile();

/* getStatus
   
   Get the status of the game.
   
   Returns 0 if there are any vacant tiles, 1 if the board is full but 
   the the player can still move, 2 if no valid moves are found (game over)
*/
unsigned char getStatus();

/* getLargestTile
   
   Get the largest tile on the board.
*/
unsigned int getLargestTile();

/* saveGame
   
   Save the board to EEPROM.
   
   Returns true if successful.
*/
bool saveGame();

/* loadGame
   
   Restore the saved game from EEPROM
   
   Returns true if successful.
*/
bool loadGame();

/* clearSave
   
   Clear the game save.
   
   Returns true if successful.
*/
bool clearSave();

// === Random number generation ===

/* getNewTileValue
   
   Get the value for a new tile.
   
   Returns 2 (90% chance) or 4 (10% chance)
*/
unsigned int getNewTileValue();

/* getNewCoord
   
   Get the coordinate for a new tile.
   
   Sets x and y to a coordinate of a random tile.
   
   *This assumes a non-full board!*
*/
void getNewCoord( unsigned char *x, unsigned char *y );

/* getRandomSeed
   
   Get a random seed.
   
   Forked from http://www.utopiamechanicus.com/article/better-arduino-random-numbers/
*/
unsigned long getRandomSeed();

// === LCD Screen ===
// print board to lcd
void printBoard();

// print the message
void printMessage();

// prints a line to LCD
void printLine( const char * str );

// read control chars via serial and take coressponding actions
// 1 if action taken (board changed), 0 if nothing done to board
bool serialControl();

// read button inputs and take corresponding actions
// 1 if action taken (board changed), 0 if nothing done
bool buttonControl();

// === Misc ===
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

// show the debug menu
void modeDebug();

// let the user adjust contrast settings 
void modeContrast();

// check if a coordinate is valid on the board
bool isCoordValid( unsigned char x, unsigned char y );

/* measureBattery
   
   Use ADC to measure the battery level. See "config.h" for more info.
   
   Returns the battery level (e.g. 90 for 90%). 0 is returned when battery
   meter is not enabled or the battery voltage is below the lowest level, which probably means that
   no battery is connected to the meter pin or the battery is dying.
   
   Heavy code smell ahead, again... Clean-up needed.
*/

unsigned char measureBattery();

// print the logo
void printLogo();

// === Basic logics ===
// init board (zero the whole array)
void initBoard();

// initialize everything
void initGame();

// == Actual code ==

// === Tile moving ===

// WARNING: Heavy code smell ahead :P
// This is just a quick'n'dirty raid, and I'll clean this up later.

/* moveOnce

   moves a tile in a direction by a tile, and returns 1 if
   moved to a empty space, 2 if merged with another tile, 0 if not
   moved at all.
   
   if noMerge is true, the function refuses to join tiles.
*/
unsigned char moveOnce( unsigned char x, unsigned char y, bool h, bool v, bool hv, bool vv, bool noMerge ) {
	unsigned char tx = x, ty = y;
	if ( h ) {
		if ( hv ) tx += 1;
		else tx -= 1;
	}
	if ( v ) {
		if ( vv	) ty += 1;
		else ty -= 1;
	}
	
	if ( !isCoordValid( x, y ) || !isCoordValid( tx, ty ) ) // illegal tile
		return 0;
	else if ( board[x][y] == 0 ) return 0; // empty tile
	else if ( board[tx][ty] == 0 ) { // the target tile is empty
		board[tx][ty] = board[x][y];
		board[x][y] = 0;
		return 1; // done!
	} else if ( !noMerge && board[tx][ty] == board[x][y] ) { // the target tile can be merged
		board[tx][ty] = 2 * board[tx][ty];
		board[x][y] = 0;
		score += board[tx][ty];
		return 2; //done!
	}
	return 0;
}

/* moveVerticallyOnce, moveHorizontallyOnce
   
   Wrappers of moveOnce() for convenience.
*/
#define moveVerticallyOnce( x, y, d, noMerge ) moveOnce( x, y, false, true, false, d, noMerge )
#define moveHorizontallyOnce( x, y, d, noMerge ) moveOnce( x, y, true, false, d, false, noMerge )

/* moveUp, moveLeft, moveDown, moveRight

   move everything to a direction, returns true if
   any tile is moved.
*/

bool moveUp() {
	unsigned char x, y, ly, lyMargin, result = 0;
	bool moved = false;
	bool noMerge = false;
	for ( x = 0; x <= 3; x++ ) {
		// now we are at a new col
		lyMargin = 0; // reset the margin
		for ( y = 1; y <= 3; y++ ) {
			// now we are at a new row
			noMerge = false; // reset the noMerge flag
			for ( ly = y; ly > lyMargin; ly-- ) {
				result = moveVerticallyOnce( x, ly, false, noMerge );
				if ( result )
					moved = true;
				if ( result == 2 ) {
					noMerge = true;
					lyMargin = ly;
				}
				if ( result == 0 ) break; // move complete
			}
		}
	}
	writeSerial( "Moved up" );
	return moved;
}

bool moveLeft() {
	unsigned char x, y, lx, lxMargin, result = 0;
	bool moved = false;
	bool noMerge = false;
	for ( y = 0; y <= 3; y++ ) {
		lxMargin = 0;
		for ( x = 1; x <= 3; x++ ) {
			noMerge = false;
			for ( lx = x; lx > lxMargin; lx-- ) {
				result = moveHorizontallyOnce( lx, y, false, noMerge );
				if ( result )
					moved = true;
				if ( result == 2 ) {
					noMerge = true;
					lxMargin = lx;
				}
				if ( result == 0 ) break; // move complete
			}
		}
	}
	writeSerial( "Moved left" );
	return moved;
}

bool moveDown() {
	unsigned char x, y, ly, lyMargin, result = 0;
	bool moved = false;
	bool noMerge = false;
	for ( x = 0; x <= 3; x++ ) {
		lyMargin = 3;
		for ( y = 2; y >= 0; y-- ) {
			noMerge = false;
			for ( ly = y; ly < lyMargin; ly++ ) {
				result = moveVerticallyOnce( x, ly, true, noMerge );
				if ( result )
					moved = true;
				if ( result == 2 ) {
					noMerge = true;
					lyMargin = ly;
				}
				if ( result == 0 ) break;
			}
			if ( y == 0 ) break; // fix the mind-bogging infinite loop
		}
	}
	writeSerial( "Moved down" );
	return moved;
}

bool moveRight() {
	unsigned char x, y, lx, lxMargin, result = 0;
	bool moved = false;
	bool noMerge = false;
	for ( y = 0; y <= 3; y++ ) {
		lxMargin = 3;
		for ( x = 2; x >= 0; x-- ) {
			noMerge = false;
			for ( lx = x; lx < lxMargin; lx++ ) {
				result = moveHorizontallyOnce( lx, y, true, noMerge );
				if ( result )
					moved = true;
				if ( result == 2 ) {
					noMerge = true;
					lxMargin = lx;
				}
				if ( result == 0 ) break; // move complete
			}
			if ( x == 0 ) break;
		}
	}
	writeSerial( "Moved right" );
	return moved;
}

// === Game process & Scoring ===

/* placeNewTile
   
   Place a new tile on the board.
*/
void placeNewTile() {
	if ( getStatus() == 0 ) { // board not full
		unsigned char x, y;
		unsigned char *px = &x;
		unsigned char *py = &y;
		getNewCoord( px, py );
		board[x][y] = getNewTileValue();
	}
}

/* getStatus
   
   Get the status of the game.
   
   Returns 0 if there are any vacant tiles, 1 if the board is full but 
   the the player can still move, 2 if no valid moves are found (game over)
*/
unsigned char getStatus() {
	bool moveable = false;
	unsigned char x, y;
	for ( y = 0; y <= 3; y++ ) {
		for ( x = 0; x <= 3; x++ ) {
			if ( board[x][y] == 0 ) { // vacant tile
				return 0; // board not full
			}
			// let's check if the player can move further
			if ( x <= 2 ) {
				if ( board[x][y] == board[x+1][y] )
					moveable = true;
			}
			if ( y <= 2 ) {
				if ( board[x][y] == board[x][y+1] )
					moveable = true;
			}
		}
	}
	// board full
	if ( moveable ) return 1; // board full, but still moveable
	else return 2; // game over!
}

/* getLargestTile
   
   Get the largest tile on the board.
*/
unsigned int getLargestTile() {
	unsigned int largest = 0;
	unsigned char x, y = 0;
	for ( y = 0; y <= 3; y++ )
		for ( x = 0; x <= 3; x++ )
			if ( board[x][y] > largest )
				largest = board[x][y];
	return largest;
}

/* saveGame
   
   Save the board to EEPROM.
   
   Returns true if successful.
*/
bool saveGame() {
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
bool loadGame() {
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
bool clearSave() {
#if enableGameSave
	unsigned char x, y;
	for ( x = 0; x <= 3; x++ )
		for ( y = 0; y <= 3; y++ )
			eeprom_write_word( &boardsave[x][y], 0 );
	eeprom_write_byte( &boardsaveflag, 0 );
	return true;
#else
	return false;
#endif
}

// === Random number generation ===

/* getNewTileValue
   
   Get the value for a new tile.
   
   Returns 2 (90% chance) or 4 (10% chance)
*/
unsigned int getNewTileValue() {
	randomSeed( getRandomSeed() );
	unsigned int tile = random( 10 ) < 9 ? 2 : 4;
	return tile;
}

/* getNewCoord
   
   Get the coordinate for a new tile.
   
   Sets x and y to a coordinate of a random tile.
   
   *This assumes a non-full board!*
*/
void getNewCoord( unsigned char *x, unsigned char *y ) {
	unsigned char i;
	for ( i = 1; i <= 20; i++ ) { // try ADC 20 times
		randomSeed( getRandomSeed() );
		(*x) = random( 4 ); // 0 to 3
		(*y) = random( 4 );
		if ( board[*x][*y] == 0 ) return;
	}
	// plan b, without using ADC...
	for ( (*x) = 0; (*x) <= 3; (*x)++ )
		for ( (*y) = 0; (*y) <= 3; (*y)++ )
			if ( board[*x][*y] == 0 ) return;
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

// === LCD Screen ===
// print board to lcd
void printBoard() {
	char buffer[4]; // store the tile string
	lcdInit();
	lcdResetCursor();
	unsigned char x, y;
	for ( y = 0; y <= 3; y++ ) {
		for ( x = 0; x <= 3; x++ ) {
			getTileString( buffer, board[x][y] );
			lcdString( buffer );
		}
	}
}

// print the message
void printMessage() {
	if ( score == 0 ) { // hasn't scored yet
		// print logo
		printLogo();
	} else { // scored
		char buffer[13]; // line buffer
#if enableBatteryMeter
		unsigned char battery = measureBattery();
		if ( battery ) {
			sprintf( buffer, "$%d &%u", score, battery );
		} else {
#endif
			sprintf( buffer, "$%d", score );
#if enableBatteryMeter
		}
#endif
		getLineString( buffer, buffer );
		lcdString( buffer );
		if ( getStatus() == 2 ) { // game over!
			printLine( "Game over!" );
		}
#if enableMessages
		else {// still alive :)
			unsigned int largest = getLargestTile();
			if ( largest > 2 && largest <= 64) { // 4 to 64
				printLine( "Great!" );
			} else if ( largest <= 512 ) { // 128 to 512
				printLine( "Go on!" );
			} else { // >= 1024
				switch ( largest ) {
					case 1024:
						printLine( "Come on!" );
						break;
					case 2048:
						printLine( "You won!" );
						break;
					case 4096:
						printLine( "Don't stop!" );
						break;
					case 8192:
						printLine( "Crazy!" );
						break;
					case 16384:
						printLine( "Wow." );
						break;
					case 32768:
						printLine( "WHAT?!" );
						break;
				}
			}
		}
#endif
	}
}

// prints a line to LCD
void printLine( const char * str ) {
	char buffer[13]; // don't modify constants!
	strncpy( buffer, str, 12 );
	getLineString( buffer, buffer );
	lcdString( buffer );
}

// read control chars via serial and take coressponding actions
// 1 if action taken (board changed), 0 if nothing done to board
bool serialControl() {
#if enableSerial
	switch( readSerial() ) {
		case -1: // nothing...
			return false;
		case 119: // "w"
			writeSerial( "Moving tiles up..." );
			return moveUp();
		case 97: // "a"
			writeSerial( "Moving tiles left..." );
			return moveLeft();
		case 115: // "s"
			writeSerial( "Moving tiles down..." );
			return moveDown();
		case 100: // "d"
			writeSerial( "Moving tiles right..." );
			return moveRight();
		case 112: // "p"
			writeSerial( "Refreshing board..." );
			printBoard();
		default:
			return false;
	}
#endif
	return false;
}

// read button inputs and take corresponding actions
// 1 if action taken (board changed), 0 if nothing done
bool buttonControl() {
	if ( isPressed( buttonUp ) ) {
		writeSerial( "Up button pressed - Moving tiles up..." );
		return moveUp();
	} else if ( isPressed( buttonLeft ) ) {
		writeSerial( "Left button pressed - Moving tiles left..." );
		return moveLeft();
	} else if ( isPressed( buttonDown ) ) {
		writeSerial( "Down button pressed - Moving tiles down..." );
		return moveDown();
	} else if ( isPressed( buttonRight ) ) {
		writeSerial( "Right button pressed - Moving tiles right..." );
		return moveRight();
	}
	return false;
}

// === Misc ===
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
		sprintf( buffer, "%-3hu", value );
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

// show the debug menu
void modeDebug() {
	lcdClear();
	printLogo();
	printLine( "Debug mode" );
	printLine( "<@ >$ ^& _x" );
	while ( true ) {
#if lcdContrastAdjustable
		// adjust contrast
		if ( isPressed( buttonLeft ) ) { // contrast
			writeSerial( "Adjusting contrast..." );
			modeContrast();
		}
#endif

#if enableGameSave
		if ( isPressed( buttonRight ) ) { // game save
			writeSerial( "Restoring game save..." );
			loadGame();
			lcdClear();
			return;
		}
#endif

#if enableBatteryMeter
		if ( isPressed( buttonUp ) ) { // battery monitor
			char buffer[13];
			unsigned char battery;
			while( true ) {
				lcdClear();
				battery = measureBattery();
				sprintf( buffer, "&%d", battery );
				lcdString( buffer );
				delay( 1000 );
			}
		}
#endif
		if ( isPressed( buttonDown ) ) {
			lcdClear();
			return;
		}
	}
}

// let the user adjust contrast settings 
void modeContrast() {
	char buffer[13];
	lcdClear();
	printLine( "contrast" );
	while( true ) { // lock the user in
		if ( isPressed( buttonRight ) ) {
			lcdSetContrast( lcdGetContrast() + 0x01 );
		} else if ( isPressed( buttonLeft ) ) {
			lcdSetContrast( lcdGetContrast() - 0x01 );
		} else if ( isPressed( buttonUp ) ) {
			lcdSetContrast( lcdDefaultContrast );
		} else continue;
		lcdInit();
		lcdClear();
		itoa( lcdGetContrast(), buffer, 10 );
		lcdString( buffer );
	}
}

// check if a coordinate is valid on the board
bool isCoordValid( unsigned char x, unsigned char y ) {
	return x <= 3 && y <= 3;
}

/* measureBattery
   
   Use ADC to measure the battery level. See "config.h" for more info.
   
   Returns the battery level (e.g. 90 for 90%). 0 is returned when battery
   meter is not enabled or the battery voltage is below the lowest level, which probably means that
   no battery is connected to the meter pin or the battery is dying.
*/
unsigned char measureBattery() {
#if enableBatteryMeter
	// let's play with some fixed point math...
	unsigned int vf, rvf, vminf, capacity, curf, pf;
	
	int adc = analogRead( pinAnalogBatteryVoltage );
	vf = (unsigned int)adc / 10.23 * analogReferenceVoltage; // Vadc * 100
	rvf = vf * batteryVoltageMultiplier; // Vreal * 100
	vminf = 100 * batteryLowestVoltage; // Vlowest * 100
	
	if ( rvf < vminf ) // lower than Vlowest
		return 0;
	else if ( rvf >= 100 * batteryVoltage ) // higher than Vbat
		return 100;
	else {
		capacity =  batteryVoltage - batteryLowestVoltage;
		curf = rvf - vminf; // ( Vreal - Vlowest ) * 100
		pf = curf / capacity;
		return pf;
	}
#else
	return 0;
#endif
}

// print the logo
void printLogo() {
#if enableFancyLogo
	unsigned char i = 0;
	lcdWrite( LCD_D, 0x00 );
	for ( i = 0; i < 2 * 84; i++ )
		lcdWrite( LCD_D, pgm_read_byte( &( bitmapLogo[i] ) ) );
	lcdWrite( LCD_D, 0x00 );
#else
	printLine( "Go for 1024!" );
	printLine( "by Zhaofeng" );
#endif
}

// === Basic logics ===
// init board (zero the whole array)
void initBoard() {
	unsigned char x, y;
	for ( x = 0; x <= 3; x++ )
		for ( y = 0; y <= 3; y++ )
			board[x][y] = 0;
}

// initialize everything
void initGame() {
	// init serial
	initSerial();
	writeSerial( "1024 by Zhaofeng Li - Initializing" );
		
	// init ADC
	analogReference( analogReferenceType );
	
	// i/o directions
	pinMode( pinButtonUp, INPUT_PULLUP );
	pinMode( pinButtonLeft, INPUT_PULLUP );
	pinMode( pinButtonDown, INPUT_PULLUP );
	pinMode( pinButtonRight, INPUT_PULLUP );
	writeSerial( "I/O directions set" );
	
	// init lcd
	lcdInitIo();
	lcdLoadContrast();
	if ( digitalRead( pinButtonRight ) == LOW ) { // right button pushed
		digitalWrite( pinLcdBl, HIGH );
		//analogWrite( pinLcdBl, 125 );
		backlightOn = true;
		writeSerial( "Backlight enabled" );
	}
	lcdInit();
	lcdClear();
	lcdCharacter( '.' );
	writeSerial( "LCD initialized" );

	// init debouncer
	initDebouncer();
	writeSerial( "Debouncers initialized" );
	lcdCharacter( '.' );
	
	// init board
	initBoard();
	placeNewTile();
	placeNewTile();
	writeSerial( "Board ready" );
	lcdCharacter( '.' );

#if enableDebugMode
	// debug mode
	if ( digitalRead( pinButtonDown ) == LOW ) {
		modeDebug();
	}
#else
#if lcdContrastAdjustable
	// contrast mode
	if ( digitalRead( pinButtonDown ) == LOW ) { // down button pushed
		modeContrast();
	}
#endif
#endif

	// init display
	lcdClear();
	printBoard();
	printMessage();
	writeSerial( "Board printed" );
}

// == References ==
// {{Reflist}} ;P
