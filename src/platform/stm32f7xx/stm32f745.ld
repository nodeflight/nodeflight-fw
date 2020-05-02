/* CPU: stm32f745 */

/*
Resources available:

RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 256K
FLASH (rx)     : ORIGIN = 0x08000000, LENGTH = 1024K

Extra RAM for real time data:
TCM_RAM (xrw)  : ORIGIN = XXXXXXXXXX, LENGTH = 32K
*/

MEMORY
{
RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 256K
FLASH (rx)     : ORIGIN = 0x08000000, LENGTH = 1024K
}

INCLUDE "stm32f7xx.ld"