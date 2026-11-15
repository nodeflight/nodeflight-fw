/*
 * The linker script of the native target: an addition to the default script
 * of the host's linker, given with -Wl,-T,native.ld. It adds what
 * src/platform/stm32/linker/STM32.ld gives the core: the tables of the
 * peripherals, resources and modules, the heap of FreeRTOS, and the region of
 * the internal disk (l1conf).
 */

/*
 * The tables, each sorted by name as on the STM32 chips. They hold pointers,
 * which the dynamic linker relocates in a position independent executable,
 * so they follow .data.rel.ro: written at the start, read-only after it
 * (RELRO). Each table is its own output section, so its start symbol is at
 * its first entry whatever the alignment of the entries.
 */
SECTIONS
{
  .nf_peripheral :
  {
    PROVIDE_HIDDEN (__nf_peripheral_start = .);
    KEEP (*(SORT(.nf_peripheral.*)))
    KEEP (*(.nf_peripheral*))
    PROVIDE_HIDDEN (__nf_peripheral_end = .);
  }

  .nf_resource :
  {
    PROVIDE_HIDDEN (__nf_resource_start = .);
    KEEP (*(SORT(.nf_resource.*)))
    KEEP (*(.nf_resource*))
    PROVIDE_HIDDEN (__nf_resource_end = .);
  }

  .nf_module :
  {
    PROVIDE_HIDDEN (__nf_module_start = .);
    KEEP (*(SORT(.nf_module.*)))
    KEEP (*(.nf_module*))
    PROVIDE_HIDDEN (__nf_module_end = .);
  }
}
INSERT AFTER .data.rel.ro;

/*
 * Memory that the program fills at run time, zeroed at the start as .bss:
 * the heap of FreeRTOS (integration/heap.c), and the internal disk, which
 * platform_init loads from the image file of NODEFLIGHT_L1CONF.
 */
SECTIONS
{
  .nf_heap (NOLOAD) : ALIGN(16)
  {
    PROVIDE_HIDDEN (__heap_start = .);
    . += 1M;
    PROVIDE_HIDDEN (__heap_end = .);
  }

  .nf_l1conf (NOLOAD) : ALIGN(16)
  {
    PROVIDE_HIDDEN (__l1conf_start = .);
    . += 128K;
    PROVIDE_HIDDEN (__l1conf_end = .);
  }
}
INSERT AFTER .bss;
