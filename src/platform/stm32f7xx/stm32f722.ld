/* CPU: stm32f722ze */

/*
Peripherals available:

RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 256K
FLASH (rx)     : ORIGIN = 0x08000000, LENGTH = 512K
*/

MEMORY
{
RAM (xrw)         : ORIGIN = 0x20000000, LENGTH = 256K
FLASH_APP (rx)    : ORIGIN = 0x08000000, LENGTH = 384K
FLASH_L1CONF (rx) : ORIGIN = 0x08060000, LENGTH = 128K
}

_Main_Stack_Size = 1024;

PROVIDE_HIDDEN(__l1conf_start = ORIGIN(FLASH_L1CONF));

INCLUDE "stm32.ld"