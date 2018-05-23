#!/bin/bash
# Setup a base directory:
SJBASE=/home/peter/SJSU-Dev

# SJSUOne Board Settings:
SJDEV=/dev/ttyUSB0 # Set this to your board ID
SJBAUD=38400

# Project Target Settings:
# Sets the binary name, defaults to "firmware" (Optional)
SJPROJ=firmware

# Sets which DBC to generate, defaults to "DBG"
ENTITY=DBG

# Compiler and library settings:
# Selects compiler version to use
PATH=$PATH:$SJBASE/tools/gcc-arm-none-eabi-6-2017-q2-update/bin
SJLIBDIR="$SJBASE/firmware/lib"

# Export everything to the environment
export SJBASE
export SJDEV
export SJBAUD
export SJPROJ
export ENTITY
export PATH
export SJLIBDIR
