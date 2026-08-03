ENTRY(Reset_Handler)

MEMORY {
  FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 512K
  SRAM(rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

/*This defines the start of the stack aka the end of SRAM, because stack starts 
from higher memory and grows to lower memory.*/
_estack = ORIGIN(SRAM) + LENGTH(SRAM);

SECTIONS {
  .isr_vector : {
    /*Stops garbage collection from deleting the vector table, because we still need to
    know where all the important stuff is in memory*/
    KEEP(*(.isr_vector));
    /*Align the end so it completed 4 bytes, so the next section in memory is 
    correctly alligned, basically padding*/
    . = ALIGN(4);
  } > FLASH

  .text : {
    *(.text*)
    . = ALIGN(4);
  } > FLASH

  .rodata : {
    *(.rodata*)
    . = ALIGN(4);
  } > FLASH

  .data : {
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
    /*Load into SRAM at VMA, but before that load into Flash during LMA*/
  } > SRAM AT > FLASH

/*Usually LOADADDR doesn't differentiate between VMA and LMA, until you use AT
in the output section, which we did above*/
  _sidata = LOADADDR(.data);

  .bss : {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    /*Heap start right after .bss and grows to higher memory until it meets the
    the stack.*/
  } > SRAM
}

_heap_start = .;
_heap_end = _estack;