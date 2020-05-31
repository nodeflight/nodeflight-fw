#!/bin/bash -e

# Test program to flash internal file system

if [ "$1" == "" ]; then
	echo "Usage: $0 <fat disk image>"
	exit 1
fi

st-flash --reset write $1 0x08060000
