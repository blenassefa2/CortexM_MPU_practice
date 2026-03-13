__STACK_SIZE_PER_CPU = 0x00004000;  /* 16K per CPU */

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
    KEEP(*(.vectors*))
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

  /* Two separate stacks, one per CPU */
  __StackTop_CPU0   = ORIGIN(NS_RAM) + LENGTH(NS_RAM);
  __StackLimit_CPU0 = __StackTop_CPU0 - __STACK_SIZE_PER_CPU;
  __StackTop_CPU1   = __StackLimit_CPU0;
  __StackLimit_CPU1 = __StackTop_CPU1 - __STACK_SIZE_PER_CPU;

  /* Keep the original names for CPU0 startup code */
  __StackTop   = __StackTop_CPU0;
  __StackLimit = __StackLimit_CPU0;
}
