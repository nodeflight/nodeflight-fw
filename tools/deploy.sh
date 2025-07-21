#!/bin/bash -xe

DEPLOYMENT=$1
DEPLOYMENT_BUILD=build/deployments/$(basename $DEPLOYMENT)

mkdir -p $DEPLOYMENT_BUILD

source $DEPLOYMENT/hwinfo.cfg

ELF_FILE=build/nodeflight-$TARGET.elf

make $ELF_FILE

L1CONF_ADDR_START=$( \
    arm-none-eabi-readelf -s $ELF_FILE | \
    sed -n -E 's/^.*: ([0-9a-f]+) .*__l1conf_start.*$/\1/p'
)
L1CONF_ADDR_END=$( \
    arm-none-eabi-readelf -s $ELF_FILE | \
    sed -n -E 's/^.*: ([0-9a-f]+) .*__l1conf_end.*$/\1/p'
)
L1CONF_SIZE=$(( 16#$L1CONF_ADDR_END - 16#$L1CONF_ADDR_START ))

if [ $L1CONF_SIZE -gt 1000000 ]; then
    echo "L1CONF_SIZE is unreasonable big - ${L1CONF_SIZE} bytes" >&2
    exit 1
fi

L1CONF_IMG=$DEPLOYMENT_BUILD/l1conf.img

./tools/dir2exfat.sh $L1CONF_SIZE NFL1CONF $DEPLOYMENT $L1CONF_IMG

arm-none-eabi-objcopy -O ihex $ELF_FILE \
    $DEPLOYMENT_BUILD/prog.hex
arm-none-eabi-objcopy -I binary -O ihex \
    --change-addresses 0x$L1CONF_ADDR_START \
    $L1CONF_IMG $DEPLOYMENT_BUILD/l1conf.hex

# TODO: is it possible to use one single write?
st-flash --reset --format ihex write $DEPLOYMENT_BUILD/prog.hex
st-flash --reset --format ihex write $DEPLOYMENT_BUILD/l1conf.hex
