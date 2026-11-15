# NodeFlight compilation and installation

Recommened using a GNU/linux based system or MacOS. No support given for installing software. The software is well documented on the internet.

Following programs are required:
- `arm-none-eabi-gcc` - gcc for embedded arm - up to date
- `pb` - [Lead Build System](https://github.com/lead-build/lead-build), generates `build.ninja`
- `ninja`

Following programs are recommedned:
- `arm-none-eabi-gdb` - debugger

For loading to a devboard, use an ARM programmer of choice. For ST-link, install:
- `stlink` - programmer and debugger interface

For a more generic programmer, [Segger J-Link](https://www.segger.com/jlink/) is recommended, with options for non-profit use.

The build uses lead-lib as a git submodule. Check out the submodules first:
```
git submodule update --init
```

To configure and compile all targets (generates `build.ninja` and `compile_commands.json`, then runs ninja):
```
./build.sh
```

After configuring once with `./configure.sh`, compile with `ninja`. To compile a given target only:
```
ninja stm32f722xe
```

The firmware of each target is written to `out/<target>/`: `nodeflight.elf`, and its images `nodeflight.elf.bin` and `nodeflight.elf.hex`. Objects are in `build/<target>/`.

To flash, using st-link:
```
st-flash --reset --format ihex write out/stm32f722xe/nodeflight.elf.hex
```

Loading a layer1 config: (currently hardcoded for stm32f722ze memory map, for layer1 config, see below)
```
./tools/flash_config.sh config_int.dmg
```
