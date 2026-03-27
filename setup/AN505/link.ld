__STACK_SIZE = 0x00008000;  /* 32K — can't exceed NS_RAM size */

MEMORY
{
   NS_FLASH     (rx)  : ORIGIN = 0x00200000, LENGTH = 512K
   NS_RAM       (rwx) : ORIGIN = 0x28200000, LENGTH = 32K
}

ENTRY(Reset_Handler)

SECTIONS
{
  .text :
  {
    KEEP(*(.vectors))
    *(.text*)
  } > NS_FLASH

  .ARM.exidx :
  {
    __exidx_start = .;
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    __exidx_end = .;
  } > NS_FLASH

  _etext = .;

  .data :
  {
    *(.data*)
  } > NS_RAM AT> NS_FLASH

  .bss :
  {
    *(.bss*)
  } > NS_RAM

  /* Stack at top of RAM — stack grows downward */
  __StackTop   = ORIGIN(NS_RAM) + LENGTH(NS_RAM);
  __StackLimit = __StackTop - __STACK_SIZE;
}