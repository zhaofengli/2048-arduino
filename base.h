/*
	2048 for Arduino
	Game manager
	
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

#ifndef BASE_H
#define BASE_H

// == Includes ==
#include <Arduino.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "board.h"
#include "battery.h"
#include "format.h"
#include "random.h"
#include "gamesave.h"
#include "display.h"
#include "button.h"
#include "serial.h"
#include "bitmap.h"

// == Runtime variables ==

// == Function prototypes ==

// === Game process ===
/* placeNewTile
   
   Place a new tile on the board.
*/
void placeNewTile();

// === LCD Screen ===
// print board to lcd
void printBoard();

// print the message
void printMessage();

// read control chars via serial and take coressponding actions
// 1 if action taken (board changed), 0 if nothing done to board
bool serialControl();

// read button inputs and take corresponding actions
// 1 if action taken (board changed), 0 if nothing done
bool buttonControl();

// show the debug menu
void modeDebug();

// print the logo
void printLogo();

// === Basic logics ===
// init board (zero the whole array)
void initBoard();

// initialize everything
void initGame();

// main game loop
void loopGame();

// == Actual code ==

// === Game process ===

/* placeNewTile
   
   Place a new tile on the board.
*/
void placeNewTile() {
	if ( getStatus() == 0 ) { // board not full
		unsigned char x, y, i;
		for ( i = 1; i <= 20; i++ ) {
			getRandomCoord( &x, &y );
			if ( !board[x][y] ) {
				 board[x][y] = getRandomTileValue();
				 return;
			}
		}
		for ( y = 0; y <= 3; y++ ) {
			for ( x = 0; x <= 3; x++ ) {
				if ( !board[x][y] ) {
					 board[x][y] = getRandomTileValue();
					 return;
				}
			}
		}
	}
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
			sprintf( buffer, "$%u &%u", score, battery );
		} else {
#endif
			sprintf( buffer, "$%u", score );
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

// show the debug menu
void interactiveDebug() {
	lcdClear();
	printLogo();
	printLine( "Debug mode" );
	printLine( "<@ >$ ^& _x" );
	while ( true ) {
		// adjust contrast
		if ( isPressed( buttonLeft ) ) { // contrast
			writeSerial( "Adjusting contrast..." );
			lcdInteractiveContrast();
		}

#if enableGameSave
		if ( isPressed( buttonRight ) ) { // game save
			writeSerial( "Restoring game save..." );
			loadGame( board );
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
				sprintf( buffer, "&%u", battery );
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
		interactiveDebug();
	}
#else
#if lcdContrastAdjustable
	// contrast mode
	if ( digitalRead( pinButtonDown ) == LOW ) { // down button pushed
		lcdInteractiveContrast();
	}
#endif
#endif

	// init display
	lcdClear();
	printBoard();
	printMessage();
	writeSerial( "Board printed" );
}

void loopGame() {
	unsigned int last = 0, cur; // largest tile
	for ( ; ; ) {
		if ( serialControl() || buttonControl() ) { // action taken
			placeNewTile();
			printBoard();
			printMessage();
			cur = getLargestTile();
			if ( cur > last ) {
				last = cur;
				if ( cur >= saveThreshold )
					saveGame( board );
			}
		}
	}
}

// == References ==
// {{Reflist}} ;P

#endif
