/*
PygmyOS linker subscript for STM32F100 with 64K Flash and 8K RAM
PygmyOS Bootloader has first 8KB reserved
PygmyOS Must be located at 0x08002000
*/

/* Memory Spaces Definitions */

MEMORY
{
  RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 8K
  FLASH (rx) : ORIGIN = 0x8002000, LENGTH = 56K
  STARTFLASH (rx) : ORIGIN = 0x0, LENGTH = 0x0
  CRPPATCH (r) : ORIGIN = 0x0, LENGTH = 0
  FLASHPATCH (r) : ORIGIN = 0x00000000, LENGTH = 0
  ENDFLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 0
  FLASHB1  (rx)  : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB0 (rx)  : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB1 (rx)  : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB2 (rx)  : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB3 (rx)  : ORIGIN = 0x00000000, LENGTH = 0
}

/* higher address of the user mode stack */
_estack = 0x20002000;

