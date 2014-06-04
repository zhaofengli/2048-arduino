/*
	2048 for Arduino
	format.h tests
	
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
#include <string.h>
#include <assert.h>
#include "../format.h"

void testTruncate() {
	printf( "testTruncate\n" );
	char buffer[20];
	getLineString( buffer, "123456789012345" );
	assert( !strcmp( buffer, "123456789012" ) );
	printf( "." );
	printf( "\ntestTruncate successful.\n" );
}

void testPadding() {
	printf( "testPadding\n" );
	char buffer[4];
	getTileString( buffer, 2 );
	assert( !strcmp( buffer, "2  " ) );
	printf( "." );
	
	getTileString( buffer, 32 );
	assert( !strcmp( buffer, "32 " ) );
	printf( "." );
	
	getTileString( buffer, 128 );
	assert( !strcmp( buffer, "128" ) );
	printf( "." );
	
	printf( "\ntestPadding successful.\n" );
}

int main() {
	printf( "format.h test\n" );
	testTruncate();
	testPadding();
	printf( "All good.\n" );
}