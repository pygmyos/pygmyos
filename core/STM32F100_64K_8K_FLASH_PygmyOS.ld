/*
PygmyOS linker subscript for STM32F100 with 64K Flash and 8K RAM
PygmyOS Bootloader has first 8KB reserved
PygmyOS Must be located at 0x08001000
*/
/* include the common STMxxx sub-script */
INCLUDE "STM32F10x_COMMON.ld"

/* include the memory spaces definitions sub-script */
INCLUDE "STM32F100_64K_8K_PygmyOS.ld"

/* include the sections management sub-script for FLASH mode */
INCLUDE "sections_FLASH.ld"
