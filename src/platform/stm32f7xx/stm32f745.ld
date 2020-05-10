/* CPU: stm32f745 */

/*
Peripherals available:

RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 256K
FLASH (rx)     : ORIGIN = 0x08000000, LENGTH = 1024K

Extra RAM for real time data:
TCM_RAM (xrw)  : ORIGIN = XXXXXXXXXX, LENGTH = 32K
*/

MEMORY
{
RAM (xrw)         : ORIGIN = 0x20000000, LENGTH = 256K
FLASH_APP (rx)    : ORIGIN = 0x08000000, LENGTH = 1016K
FLASH_L1CONF (rx) : ORIGIN = 0x080fe000, LENGTH = 8K
}

/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 8K;     /* required amount of heap  */
_Min_Stack_Size = 0x400; /* required amount of stack */

PROVIDE_HIDDEN(__l1conf_start = ORIGIN(FLASH_L1CONF));

INCLUDE "stm32.ld"