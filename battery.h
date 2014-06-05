/*
	2048 for Arduino
	Battery meter
	
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

#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>
#include <math.h>
#include "config.h"

// == Function prototypes ==
/* measureBattery
   
   Use ADC to measure the battery level. See "config.h" for more info.
   
   Returns the battery level (e.g. 90 for 90%). 0 is returned when battery
   meter is not enabled or the battery voltage is below the lowest level, which probably means that
   no battery is connected to the meter pin or the battery is dying.
*/
unsigned char measureBattery();

// == Actual code ==

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

#endif
