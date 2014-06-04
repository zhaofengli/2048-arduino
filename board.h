/*
	2048 for Arduino
	Board & Tile moving
	
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

// the board
unsigned int board[4][4];

// current score
unsigned int score;

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
//#define moveVerticallyOnce( x, y, d, noMerge ) moveOnce( x, y, false, true, false, d, noMerge )
//#define moveHorizontallyOnce( x, y, d, noMerge ) moveOnce( x, y, true, false, d, false, noMerge )

/* moveUp, moveLeft, moveDown, moveRight

   move everything to a direction, returns true if
   any tile is moved.
*/
bool moveUp();
bool moveLeft();
bool moveDown();
bool moveRight();

// === Game process & Scoring ===

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

// check if a coordinate is valid on the board
bool isCoordValid( unsigned char x, unsigned char y );


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
	return moved;
}

// === Game process & Scoring ===


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

// check if a coordinate is valid on the board
bool isCoordValid( unsigned char x, unsigned char y ) {
	return x <= 3 && y <= 3;
}

