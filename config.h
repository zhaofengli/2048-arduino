/*
	2048 for Arduino
	Configuations
	
	This file is provided AS IS without any warranty.
*/
// == Configuations ==

// === LCD configuation ===
// This connection obstructs the serial interface
// Set enableSerial to 0 when you use this one!
#define pinLcdReset    0
#define pinLcdSce      1
#define pinLcdDc       2
#define pinLcdSdin     3
#define pinLcdSclk     4
#define pinLcdBl       6

/* lcdDefaultContrast
   
   Lower value for lower contrast, and vice versa.
   The actual contrast varies with voltage!
*/
#define lcdDefaultContrast    0xB4

/* lcdContrastStep
   
   Contrast added when backlight is on.
*/
#define lcdContrastStep  0x05

/* 
   lcdContrastAdjustable
   
   Whether the contrast can be adjusted at runtime
   (saved in EEPROM)
*/
#define lcdContrastAdjustable 1

// === Button connection ===
#define pinButtonUp    10
#define pinButtonLeft  8
#define pinButtonDown  7
#define pinButtonRight 9

// === Random number generation ===
/*  pinRandomSource
    
    An unconnected analog pin for entropy generation.
*/
#define pinAnalogRandomSource 3

/* randomBits
   
   Bits of the random seed. Longer seeds produce less 
   bias, but may slow things down.
*/
#define randomBits 5

/* randomDebiasLimit
   
   Limit the attempts to debias the random seed. Bigger 
   values produce less bias, but may slow things down.
*/
#define randomDebiasLimit 3

// === Debouncer ===
/* buttonDebounceInterval
   
   The debounce interval, in ms. Increase this if your buttons are 
   very "bouncy".
*/
#define buttonDebounceInterval 5

// === ADC ===
/* analogReferenceType
   
   The ADC reference to use. This can be one of: 
   DEFAULT, INTERNAL, INTERNAL1V1, INTERNAL2V56 and EXTERNAL.
   
   See the documentations of analogReference() on Arduino wiki 
   for more info.
*/
#define analogReferenceType INTERNAL

/* analogReferenceVoltage
   
   The ADC reference voltage, in volts.
   
   The INTERNAL analog reference is 1.1V on ATmega328 and ATmega168, and 
   2.56V on ATmega8.
*/
#define analogReferenceVoltage 1.1

// === Battery meter ===
/* enableBatteryMeter
   
   If 1, this will enable the battery meter and displays the
   battery percentage besides the score.
*/
#define enableBatteryMeter 1

/* batteryVoltage
   
   The voltage of the battery when full, in volts.
*/
#define batteryVoltage 9

/* batteryEmptyVoltage
   
   The lowest voltage of the battery, in volts.
*/
#define batteryLowestVoltage 7

/* batteryVoltageMultiplier
   
   The voltage multiplier.
   
   For example, if the multiplier is 2, the measured voltage will be doubled.
*/
#define batteryVoltageMultiplier 25

/* pinAnalogBatteryVoltage
   
   An analog pin connected directly to (divided) battery voltage.
*/
#define pinAnalogBatteryVoltage 5

// === Serial connection ===
/*  enableSerial
    
    If 1, serial support will be enabled, and
    the board will listen to 'w', 'a', 's', 'd' on the
    serial interface and take appropiate actions. Be aware
    that the Serial library is quite bloated.
    
    If 0, the Serial library and strings will not 
    be complied into the binary at all, saving /a lot/ of memory.
*/
#define enableSerial 0

/* serialBaud
   
   The baud rate of the serial interface
*/
#define serialBaud 9600

// == In-game messages ==
/* enableMessages
   
   If 1, a message will be displayed
   at the bottom of the screen, which is changed as
   the player progresses. See printMessage()
   in "base.h".
*/
#define enableMessages 1

// == Debug mode ==
/* enableDebugMode
   
   If 1, a debug menu is accessible by holding
   the down key when powering on. This includes 
   an option to change the contrast and a battery monitor.
   
   If 0 but lcdContrastAdjustable is enabled,
   holding the down key when powering on will enter
   the contrast adjustment mode.
*/
#define enableDebugMode 1

// == Fancy logo ==
/* enableFancyLogo
   
   If 1, a bitmap logo will be displayed when
   the game starts. Otherwise, the text version will be
   displayed instead.
*/
#define enableFancyLogo 1

// == Game save ==
/* enableGameSave
   
   If 1, the game will be saved every time when the player
   achieves a higher tile bigger than 256.
*/
#define enableGameSave 1

// == Bounce library ==
/* enableBounceLibrary
   
   Enable the Bounce library which can debounce buttons.
   
   If 1, the Bounce library will be included. Please uncomment
   the line in the "Libraries" section of the Makefile to build.
   If 0, a simple button handler will be used instead which does
   not have debouncing ability. This reduces build size.
   
   See button.h for more info.
*/
#define enableBounceLibrary 1
