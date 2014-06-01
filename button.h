/*
	2048 for Arduino
	Button handling

	This provides a clean wrapper for the Bounce library, 
	as well as a simple button handling routine to be used 
	instead of the library.

	This file is provided AS IS without any warranty.
*/

// == Includes ==
#include <Arduino.h>
#include "config.h"

#if enableBounceLibrary
#include <Bounce2.h>
#endif

// == Constants ==
#define countButtons 4

#define buttonUp 0
#define buttonLeft 1
#define buttonDown 2
#define buttonRight 3

// == Runtime variables ==
unsigned char pinsButton[countButtons] = { pinButtonUp, pinButtonLeft, pinButtonDown, pinButtonRight };

#if enableBounceLibrary
Bounce debouncers[countButtons];
#else
bool stateButtons[countButtons];
#endif

// == Function prototypes ==
void initDebouncer();
bool isPressed( unsigned char button );

// == Actual code ==
void initDebouncer() {
	unsigned char button;
	for ( button = 0; button < countButtons; button++ ) {
		pinMode( pinsButton[button], INPUT_PULLUP );
#if enableBounceLibrary
		// Bounce library
		debouncers[button] = Bounce();
		debouncers[button].attach( pinsButton[button] );
		debouncers[button].interval( buttonDebounceInterval );
		debouncers[button].update();
#else
		// Poor Man's Button Handler (tm)
		stateButtons[button] = true;
#endif
	}
}

/* isPressed
   
   Is a button pressed?

   This only returns true when the button is first pressed.
*/
bool isPressed( unsigned char button ) {
#if enableBounceLibrary
	return debouncers[button].update() && debouncers[button].read() == LOW;
#else
	if ( digitalRead( pinsButton[button] ) == LOW ) {
		if ( !stateButtons[button] ) { // button is not pushed before
			stateButtons[button] = true;
			return true;
		} else // pushed before
			return false;
	} else if ( stateButtons[button] )
		stateButtons[button] = false;
	
	return false;
#endif
}
