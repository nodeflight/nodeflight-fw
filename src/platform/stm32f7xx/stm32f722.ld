/* CPU: stm32f722ze */

/*
Resources available:

RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 256K
FLASH (rx)     : ORIGIN = 0x08000000, LENGTH = 512K
*/

MEMORY
{
RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 256K
FLASH (rx)     : ORIGIN = 0x08000000, LENGTH = 512K
}

INCLUDE "stm32f7xx.ld"