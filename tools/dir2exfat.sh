#!/bin/bash

if [ $# -ne 4 ]; then
    echo "Usage $0 <size> <label> <srcdir> <image>" >&2
    exit 1
fi

SIZE=$1
LABEL=$2
SRCDIR=$3
IMAGE=$4

rm -f $IMAGE
fallocate -l $SIZE $IMAGE
mformat -i $IMAGE -v "$LABEL"
mcopy -i $IMAGE -s $SRCDIR/* ::

# To also list files:
# mdir -i $IMAGE -/