#!/bin/bash

# Builds and runs nodeflight on the build machine: the program of the target
# native, with the image of the internal disk made from deployments/native.
# Arguments are passed on to the program.

set -e

cd "$(dirname "$0")/.."

DEPLOYMENT=deployments/native
DEPLOYMENT_BUILD=build/deployments/native
PROGRAM=out/native/nodeflight
L1CONF_IMG=$DEPLOYMENT_BUILD/l1conf.img

# The size of the region __l1conf_start to __l1conf_end of
# src/platform/native/native.ld
L1CONF_SIZE=131072

./configure.sh
ninja $PROGRAM

mkdir -p $DEPLOYMENT_BUILD
./tools/dir2exfat.sh $L1CONF_SIZE NFL1CONF $DEPLOYMENT $L1CONF_IMG

NODEFLIGHT_L1CONF=$L1CONF_IMG exec $PROGRAM "$@"
