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
./tools/flash_config.sh test_l1conf.txt
```

## Building blocks

NodeFlight is implemented on top of FreeRTOS, which means FreeRTOS terminology is used for FreeRTOS specific functionality.

NodeFlight is built up of a set of main compoonents:

| Component  | Short | Description                                                           | Comment                             |
| ---------- | ----- | --------------------------------------------------------------------- | ----------------------------------- |
| Peripheral | pp    | An generic peripheral, enabling access to I/O or similar. UARTs, SPI  | Implementation specific to platform |
| Resource   | rs    | A resource shared between peripherals. For example PINs, DMA          | Implementation specific to platform |
| Interface  | if    | An instance of a peripheral, for example UART3 with given pins        | Created by configuration            |
| Module     | md    | A type of node, which adds functionality to an instance               | Created by configuration            |
| Schedule   | sh    | A set of operations, added by modules, to be executed a ga given rate | Not implemented                     |

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
- `mod <type> <configuration>` - Load a module

Example:

```
per uart_st uart3 pin_d08 pin_d09 dma_1_3 dma_1_1
mod stdout uart_st
```

The example registers a configuration for an _interface_ in the first line, and creates an stdout _module_ using the an _interface_ defined in the previous line, connecting stdout to uart3. (adding debug output functionality)

Note that the `per` line just creates the template for the interface, but doesn't create the interface. This is important for building multiple `per`-lines using the same resource, even though only one of the interfaces is created. It makes it possible to add a template configuration for hardware, followed by application configuration for functionality afterwards, for separation.

## Configuration storage

Configuration is layered, where each layer is loaded in sequence. A previous layer may enable access to lower layers.

Functionality placed in different layers are not enforced. The layered structure is just a recommendation and best practice.

For smaller embedded control systems, it may for example be suitable to put all configuration in layer 1, where no sd card is available.

### Layer 1 - onboard flash

Stored in a flash page a string

Intended to store `per` lines for on board interfaces, and create instances necessary for accessing layer 2, for example SD card drivers.

Example (may not be supported yet):
```
per spi_mpu6000 spi4 pin_e02 pin_e05 pin_e06 pin_e04 pin_e01
per spi_sdcard spi1 pin_a05 pin_a06 pin_a07 pin_a04 pin_d08
mod sdcard spi_sdcard
mod config_file layer2.txt
```

### Layer 2 - user config loader

Stored preferably on an SD card, but can vary dependent on board.

Stores user interface configuration and hardware mapping. modules for I/O are instanced.

Intended to load application/regulator configuration

Example:
```
mod gyro_mpu6000 spi_mpu6000 ...
mod config_file layer3.txt
```

### Layer 3 - control logic

Calculation blocks, which interfaces the I/O blocks provided by layer 2, or each other.

No hardware dependent interfaces should be placed in layer 3, to enable re-use between different boards.

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
