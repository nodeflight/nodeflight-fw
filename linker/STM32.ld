ENTRY(Reset_Handler)

/* End of stack */
_estack = ORIGIN(RAM) + LENGTH(RAM);

/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 8K;     /* required amount of heap  */
_Main_Stack_Size = 1K;    /* required amount of stack */

PROVIDE_HIDDEN(__l1conf_start = ORIGIN(FLASH_L1CONF));
PROVIDE_HIDDEN(__l1conf_end = ORIGIN(FLASH_L1CONF) + LENGTH(FLASH_L1CONF));

SECTIONS
{
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } >FLASH_APP

  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH_APP

  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */

    . = ALIGN(4);
    PROVIDE_HIDDEN (__nf_peripheral_start = .);
    KEEP (*(SORT(.nf_peripheral.*)))
    KEEP (*(.nf_peripheral*))
    PROVIDE_HIDDEN (__nf_peripheral_end = .);

    . = ALIGN(4);
    PROVIDE_HIDDEN (__nf_resource_start = .);
    KEEP (*(SORT(.nf_resource.*)))
    KEEP (*(.nf_resource*))
    PROVIDE_HIDDEN (__nf_resource_end = .);

    . = ALIGN(4);
    PROVIDE_HIDDEN (__nf_module_start = .);
    KEEP (*(SORT(.nf_module.*)))
    KEEP (*(.nf_module*))
    PROVIDE_HIDDEN (__nf_module_end = .);

    . = ALIGN(4);
  } >FLASH_APP

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH_APP
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH_APP

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH_APP

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH_APP
  
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH_APP

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data : 
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >RAM AT> FLASH_APP

  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM

  /* Allocate space for heap */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( __heap_start = . );
    . = ORIGIN(RAM) + LENGTH(RAM) - _Main_Stack_Size;
    PROVIDE ( __heap_end = . );
  } >RAM

  

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}

