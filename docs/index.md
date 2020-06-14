# NodeFlight

**Note: in early development. Infrastructure is still under construction. Feel free to contact for more information.**

NodeFlight is a platform for building embedded generic control systems, using node diagrams.

## Compilation and installation

Since NodeFlight is a platform for building control systems, it is recommended to look through the documentation prior to usage.

Information about configuration and installation is available in the [NodeFlight compilation and installation](install.md) documentation

## Target plaforms

NodeFlight is intended to be supported by multiple Cortex-M based systems, with hardware abstraction to make the system available for microcontrollers from multiple vendors.

Performance may vary between microcontrollers, which may limit number of nodes, and update frequency.

| Plaform   | Status                                   |
| --------- | ---------------------------------------- |
| stm32f722 | supported (primary development platform) |
| stm32f745 | partial support                          |
| stm32f405 | soon                                     |

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

## Configuration

The configuration contains all mappings of the system. For information about configuration, see [NodeFlight configuration](configuration.md) for more information

# To be designed

The system is in early stage. Functionality still intended to be added:

- Signal flow configuration
- Out-of-tree modules, for application specific hardware, such as user interfaces
- configuration protocol
- named parameters to simplify configuration UI

## NFCP - NodeFlight Control Protocol

Configuration and remote control is done via the NFCP protocol. For more information, see [NodeFlight Control Protocol](nfcp.md) documentation.

# Contact

Developed and designed by:
- Max Sikström - max@pengi.se

License: GNU GPL v3 - see [LICENSE.md](LICENSE.md) for more information
