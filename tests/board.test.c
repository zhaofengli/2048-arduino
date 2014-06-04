/*
	2048 for Arduino
	board.h tests
	
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

// This is intended to be ran on your development box

#include <stdio.h>
#include <assert.h>
#include "../board.h"

#define s( x, y, v ) board[x][y] = v
#define g( x, y ) board[x][y]

void abort() {
	printf( "failed\n" );
}
void resetBoard() {
	unsigned char x, y;
	for ( x = 0; x <= 3; x++ )
		for ( y = 0; y <= 3; y++ )
			board[x][y] = 0;
}

void testMerge() {
	printf( "testMerge\n" );
	resetBoard();
	s( 1, 0, 2 );
	s( 2, 0, 2 );
	s( 3, 0, 4 );
	moveLeft();
	assert( g( 0, 0 ) == 4 && g( 1, 0 ) == 4 );
	printf( "a" );
	moveLeft();
	assert( g( 0, 0 ) == 8 );
	printf( "a" );
	
	s( 0, 1, 4 );
	s( 0, 2, 4 );
	moveDown();
	assert( g( 0, 3 ) == 8 && g( 0, 2 ) == 8 );
	printf( "s" );
	moveDown();
	assert( g( 0, 3 ) == 16 );
	printf( "s" );
	
	s( 1, 3, 8 );
	s( 2, 3, 8 );
	moveRight();
	assert( g( 2, 3 ) == 16 && g( 3, 3 ) == 16 );
	printf( "d" );
	moveRight();
	assert( g( 3, 3 ) == 32 );
	printf( "d" );
	
	s( 3, 2, 16 );
	s( 3, 1, 16 );
	moveUp();
	assert( g( 3, 0 ) == 32 && g( 3, 1 ) == 32 );
	printf( "w" );

	moveUp();
	assert( g( 3, 0 ) == 64 );
	printf( "w" );
	
	printf( "\ntestMerge successful.\n" );
}

void testScore() {
	printf( "testScore\n" );
	resetBoard();
	score = 0;
	s( 1, 0, 2 );
	s( 2, 0, 2 );
	s( 3, 0, 4 );
	moveLeft();
	assert( score == 4 );
	printf( "." );
	moveLeft();
	assert( score == 4 + 8 );
	printf( "." );
	printf( "\ntestScore successful.\n");
}

int main() {
	printf( "board.h test\n" );
	testMerge();
	testScore();
	printf( "All good.\n" );
	return 0;
}