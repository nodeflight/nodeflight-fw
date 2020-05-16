#!/bin/bash -e

# Test program to flash layer1 config to stm32f722

if [ "$1" == "" ]; then
	echo "Usage: $0 <config text file>"
	exit 1
fi

TEMPFILE=$(mktemp -t nf_l1_conf_)
cat "$1" > $TEMPFILE
dd if=/dev/zero bs=1 count=1 >> $TEMPFILE
xxd $TEMPFILE
st-flash --reset write $TEMPFILE 0x08060000
rm $TEMPFILE
