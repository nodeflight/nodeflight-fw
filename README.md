# NodeFlight

**Note: in early development. Infrastructure is still under construction. Feel free to contact for more information.**

NodeFlight is a platform for building embedded generic control systems, using node diagrams.

## Target plaforms

NodeFlight is intended to be supported by multiple Cortex-M based systems, with hardware abstraction to make the system available for microcontrollers from multiple vendors.

Performance may vary between microcontrollers, which may limit number of nodes, and update frequency.

Initially, focus on CPUs and boards used by BetaFlight compatible quadcopters.

| Plaform   | Status                                   |
| --------- | ---------------------------------------- |
| stm32f722 | supported (primary development platform) |
| stm32f745 | partial support                          |
| stm32f405 | soon                                     |
| nrf52840  | planned                                  |

## Compile and install

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
make build.stm32f722
```

To flash, using st-link:
```
st-flash --reset --format ihex write build/nodeflight-stm32f722.hex
```

Loading a layer1 config: (currently hardcoded for stm32f722ze memory map, for layer1 config, see below)
```
./tools/flash_config.sh config_int.dmg
```

## Building blocks

NodeFlight is implemented on top of FreeRTOS, which means FreeRTOS terminology is used for FreeRTOS specific functionality.

NodeFlight is built up of a set of main compoonents:

| Component  | Short | Description                                                           | Comment                             |
| ---------- | ----- | --------------------------------------------------------------------- | ----------------------------------- |
| Peripheral | pp    | An generic peripheral, enabling access to I/O or similar. UARTs, SPI  | Implementation specific to platform |
| Resource   | rs    | A resource shared between peripherals. For example PINs, DMA          | Implementation specific to platform |
| Interface  | if    | An instance of a peripheral, for example UART3 with given pins        | Created by configuration            |
| Module     | md    | A type of node, which adds functionality to an instance               |                                     |
| Schedule   | sc    | A set of operations, added by modules, to be executed a ga given rate |                                     |
| Variables  | vr    | Shared variables/links between modules                                |                                     |
| Config     | cf    | Manages the overall configuration                                     |                                     |

## Terminology

Some common names are used:

| Name  | Description                                                                                               |
| ----- | --------------------------------------------------------------------------------------------------------- |
| `tag` | Identifies an object. tags can be re-used between different types of storage, even though not recommended |

## Configuration

No intefaces or modules is created without configuration.

The configuration is stored as text, and is intended to be updated without updating nodeflight.

Each line contains an instruction:
- `per <tag> <configuration>` - peripheral, defines a template for the peripheral configuration.
- `mod <name> <type> <configuration>` - Load a module. Modules names are optional, and can be skipped by `-`
- `inc <file>` - Add file to a configuration read queue. Files are loaded one at a time, and is read in order.

Example:

```
per uart_st uart3 pin_d08 pin_d09 dma_1_3 dma_1_1
mod stdout uart_st
```

The example registers a configuration for an _interface_ in the first line, and creates an stdout _module_ using the an _interface_ defined in the previous line, connecting stdout to uart3. (adding debug output functionality)

Note that the `per` line just creates the template for the interface, but doesn't create the interface. This is important for building multiple `per`-lines using the same resource, even though only one of the interfaces is created. It makes it possible to add a template configuration for hardware, followed by application configuration for functionality afterwards, for separation.

## Configuration storage

Configuration is stored in files in a mapped file system. While configuration loads, more file systems may be mounted and gives access to more configuration.

On the internal storage, extra configuration files may be provided to load on board sensors for easy use.

For smaller systems, without access to SD card, it is possible to store all configuration in the on-board file system

## Boot sequence

1. NodeFlight mounts internal flash storage as `/int`
2. The configuration `/int/boot.cfg` is loaded
3. `/int/boot.cfg` Configure and mount an external storage, preferably as `/ext`
4. `/int/boot.cfg` adds an external configuration file as config source, preferrably `/ext/boot.cfg`
5. `/ext/boot.cfg` includes application specific configration files needed

## Best practice

On board flash is not intended to be updated by the end user, but by the board developer. A common practice between board manufacturers and application developers needs to be established.

Therefore, the following convention is used for configuration files:

### `/int/boot.cfg`
Bare minimum for configuration of external flash and load `/ext/boot.cfg`. May also start of necessary interfaces for system functionality, for example fan control, or possibly debug IO in a development environment.

Example:
```
# Setup SD card interface

per sdcard_cs gpio_a15 pin_a15
per sdcard_spi spi1 pin_a05 pin_a07 pin_a06 dma_none dma_none

mod ext sdcard sdcard_spi sdcard_cs

# Load external configuration

inc /ext/boot.cfg
```

### `/int/defaults.cfg`
Include `per` lines mapping peripherals to pins, without instancing. Should be possible to include to simplify loading of modules, without significant overhead, and without occupying resources, even though multiple `per` lines may use the same. No `mod` lines is allowed.

Those lines acts as defaults, and can be overridden by application configuration.

Example:
```
# mpu6500 interface
per mpu6500_cs gpio_d04 pin_d04
per mpu6500_int gpio_c00 pin_c00
per mpu6500_spi spi2 pin_d03 pin_c03 pin_c02 dma_none dma_none

# uart2 full duplex
per uart2_full uart2 pin_d05 pin_d06 dma_1_6 dma_1_5

# uart2 half duplex
per uart2_half uart2 pin_none pin_d06 dma_1_6 dma_1_5
```

### `/ext/boot.cfg`
Loader for application configuration. Intended to load necessary configuration, to opt-in or opt-out of using the defaults. To load files containing glue to connect hardware to control logic, and load control logic models. Intended to only contain `inc` statements.

Example:
```
inc /int/defaults.cfg
inc /ext/control-loop.cfg
inc /ext/sensors.cfg
inc /ext/telemetry.cfg
inc /ext/glue.cfg
```

# To be designed

The system is in early stage. Functionality still intended to be added:

- Signal flow configuration
- Out-of-tree modules, for application specific hardware, such as user interfaces
- configuration protocol
- named parameters to simplify configuration UI

# Contact

Developed and designed by:
- Max Sikström - max@pengi.se

License: GNU GPL v3 - see [LICENSE.md](LICENSE.md) for more information
