# NodeFlight

**Note: in early development. Infrastructure is still under construction. Feel free to contact for more information.**

NodeFlight is a platform for building embedded generic control systems, using node diagrams.

Key features for NodeFlight:
- Fast enough for quad copters
- Generic to build arbitrary control systems
- No dependency on external peripherals*
  
\* For practical reasons, it is recommended to have an SD card slot for configuration storage and logging, and a UART for configuration.

## Target plaforms

NodeFlight is intended to be supported by multiple Cortex-M based systems, with hardware abstraction to make the system available for microcontrollers from multiple vendors.

Performance may vary between microcontrollers, which may limit number of nodes, and update frequency.

| Plaform     | Status                                   |
| ----------- | ---------------------------------------- |
| STM32F722xE | supported (primary development platform) |
| STM32F745xG | partial support                          |
| STM32F405xG | soon                                     |

# Documentation

For more information, see [documentation](docs/index.md)

# Contact

Developed and designed by:
- Max Sikström - max@pengi.se

License: GNU GPL v3 - see [LICENSE.md](LICENSE.md) for more information
