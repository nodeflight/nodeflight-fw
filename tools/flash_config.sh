#!/bin/bash -e

# Test program to flash layer1 config to stm32f722

if [ "$1" == "" ]; then
	echo "Usage: $0 <config text file>"
	exit 1
fi

st-flash --reset write $1 0x08060000
