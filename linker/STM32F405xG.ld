/* CPU: STM32F405xG */

MEMORY
{
RAM (xrw)         : ORIGIN = 0x20000000, LENGTH = 128K
FLASH_APP (rx)    : ORIGIN = 0x08000000, LENGTH = 1024K - 128K;
FLASH_L1CONF (rx) : ORIGIN = 0x080e0000, LENGTH = 128K
}

INCLUDE "STM32.ld"