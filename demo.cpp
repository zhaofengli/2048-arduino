/*
	2048 for Arduino
	Demo wrapper
	
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

#include <stdio.h>
#include <stdlib.h>
#include "format.h"
#include "board.h"

void place() {
	while ( 1 ) {
		unsigned int x, y;
		x = (unsigned int)( drand48() * 4 );
		y = (unsigned int)( drand48() * 4 );
		if ( !board[x][y] ){
			if ( drand48() < 0.9 ) board[x][y] = 2;
			else board[x][y] = 4;
			break;
		}
	}
}

int main() {
	printf( "2048 for Ardui... oops, PC. :)\n" );
	place();
	while ( 1 ){
		if ( !getStatus() ) {
			place();
		} else if ( getStatus() == 2 ) {
			printf("Game over!");
			return 0;
		}
		unsigned int x, y;
		char buff[4];
		printf( "\n" );
		for ( y = 0; y <= 3; y++ ) {
			for ( x = 0; x <= 3; x++ ) {
				getTileString( buff, board[x][y] );
				printf( "%s ", buff );
			}
			printf( "\n" );
		}
		printf( "Score: %lu\n", score );
		char i;
		printf( "Now what? " );
		scanf( "%c", &i );
		if ( i == 'w' ) moveUp();
		if ( i == 'a' ) moveLeft();
		if ( i == 's' ) moveDown();
		if ( i == 'd' ) moveRight();
	}
}
