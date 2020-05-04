/* CPU: stm32f722ze */

/*
Peripherals available:

RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 256K
FLASH (rx)     : ORIGIN = 0x08000000, LENGTH = 512K
*/

MEMORY
{
RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 256K
FLASH (rx)     : ORIGIN = 0x08000000, LENGTH = 512K
}

/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 8K;     /* required amount of heap  */
_Min_Stack_Size = 0x400; /* required amount of stack */

INCLUDE "stm32.ld"