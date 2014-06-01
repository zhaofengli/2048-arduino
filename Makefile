########################################################################
# 2048 for Arduino
# Makefile
########################################################################

########################################################################
# Arduino board
#
# A board tag defined in hardware/arduino/boards.txt in your Arduino 
# IDE installation directory. For example, uno.
########################################################################
BOARD_TAG    = uno

########################################################################
# Serial device
#
# The serial device representing your Arduino board, e.g. /dev/ttyUSB0
# Not needed if you use an ISP to upload.
########################################################################
#MONITOR_PORT = /dev/ttyUSB0

########################################################################
# Arduino IDE installation dir
#
# The directory of your Arduino IDE installation. It contains 
# sub-directories named "hardware", "examples", "libraries", etc..
########################################################################
ARDUINO_DIR := /opt/arduino-1.0.5

########################################################################
# AVR toolchain dir
#
# The directory of your AVR toolchain. If you installed it via your
# package manager on Linux, it's probably /usr. If you are using 
# Windows or would like to use the one bundled with the IDE, use the
# commented-out one below.
########################################################################
#AVR_TOOLS_DIR := /usr
AVR_TOOLS_DIR := $(ARDUINO_DIR)/hardware/tools/avr

########################################################################
# Libraries
#
# If you have enableBounceLibrary enabled in config.h, uncomment the 
# following line.
########################################################################
#ARDUINO_LIBS = Bounce2

########################################################################
# Don't touch anything below! "make" and enjoy! The hex files will be 
# placed under the build/ sub-directory under this one.
########################################################################

########################################################################
# Target dir
########################################################################
OBJDIR       = build/$(BOARD_TAG)

########################################################################
# Additional complier flags
#
# -mcall-prologues saves ~200b :)
########################################################################
CPPFLAGS     = -mcall-prologues

########################################################################
# Main Arduino.mk
########################################################################
include /opt/Arduino-Makefile/Arduino.mk
