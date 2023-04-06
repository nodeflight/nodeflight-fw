# NodeFlight compilation and installation

Recommened using a GNU/linux based system or MacOS. No support given for installing software. The software is well documented on the internet.

Following programs are required:
- `arm-none-eabi-gcc` - gcc for embedded arm - up to date
- `make`

Following programs are recommedned:
- `arm-none-eabi-gdb` - debugger

For loading to a devboard, use an ARM programmer of choice. For ST-link, install:
- `stlink` - programmer and debugger interface

For a more generic programmer, [Segger J-Link](https://www.segger.com/jlink/) is recommended, with options for non-profit use.

To compile for a given platform:
```
make nodeflight-stm32f722.elf
```
or
```
make nodeflight-stm32f722.hex
```

To flash, using st-link:
```
st-flash --reset --format ihex write build/nodeflight-stm32f722.hex
```

Loading a layer1 config: (currently hardcoded for stm32f722ze memory map, for layer1 config, see below)
```
./tools/flash_config.sh config_int.dmg
```
