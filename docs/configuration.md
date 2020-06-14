# NodeFlight configuration

All configuration is store in text files on a virtual file system.

## Configuration storage

Configuration is stored in files in a mapped file system. While configuration loads, more file systems may be mounted and gives access to more configuration.

On the internal storage, extra configuration files may be provided to load on board sensors for easy use.

For smaller systems, without access to SD card, it is possible to store all configuration in the on-board file system

## Terminology

Some common names are used:

| Name  | Description                                                                                               |
| ----- | --------------------------------------------------------------------------------------------------------- |
| `tag` | Identifies an object. tags can be re-used between different types of storage, even though not recommended |

## Configuration files

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
