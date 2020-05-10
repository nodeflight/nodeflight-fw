/* CPU: stm32f405 */

/*
Peripherals available:

RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 128K (+ 64kb somewhere else...)
FLASH (rx)     : ORIGIN = 0x08000000, LENGTH = 1024K
*/

MEMORY
{
RAM (xrw)         : ORIGIN = 0x20000000, LENGTH = 128K
FLASH_APP (rx)    : ORIGIN = 0x08000000, LENGTH = 896K
FLASH_L1CONF (rx) : ORIGIN = 0x080e0000, LENGTH = 128K
}

/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 8K;     /* required amount of heap  */
_Min_Stack_Size = 0x400; /* required amount of stack */

PROVIDE_HIDDEN(__l1conf_start = ORIGIN(FLASH_L1CONF));

INCLUDE "stm32.ld"