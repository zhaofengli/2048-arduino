/*
	2048 for Arduino
	Serial wrapper
	
	This file is provided AS IS without any warranty.
*/

#ifndef SERIAL_H
#define SERIAL_H

#include "config.h"

/*
   These functions are wrapped by preprocessors. They should be used
   instead of the built-in ones supplied by Arduino.
*/

// == Serial wrapper ==
// forgive me for these dirty preprocessor dirs
// BUT, this indeed can save a lot of space when serial is disabled.
#if enableSerial
#define initSerial() initSerialReal()
#define writeSerial(s) writeSerialReal(s)
#define readSerial() readSerialRead()
#else
#define initSerial() 
#define writeSerial(s) 
#define readSerial() 
#endif

// initialize the serial interface
// Call this as initSerial()!
void initSerialReal() {
#if enableSerial
	Serial.begin( serialBaud );
#endif
}

// write a message to serial
// Call this as writeSerial( String message )!
void writeSerialReal( String message ) {
#if enableSerial
	Serial.println( message );
#endif
}

// read a byte from serial
// always returns -1 if serial is not enabled
// Call this as readSerial()!
int readSerialReal() {
#if enableSerial
	return Serial.read(); // serial enabled
#else
	return -1; // serial not enabled
#endif
}

#endif
