/*
 * STM32F407VG Linker Script
 * Cortex-M4
 *
 * Flash : 1 MB
 * SRAM  : 128 KB
 */

ENTRY(Reset_Handler)

/* Top of RAM */
_estack = ORIGIN(RAM) + LENGTH(RAM);

_Min_Heap_Size  = 0x2000;
_Min_Stack_Size = 0x4000;

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
    /*
     * Interrupt Vector Table
     */
    .isr_vector :
    {
        . = ALIGN(4);

        KEEP(*(.isr_vector))

        . = ALIGN(4);
    } > FLASH


    /*
     * Program code
     */
    .text :
    {
        . = ALIGN(4);

        *(.text)
        *(.text*)

        *(.glue_7)
        *(.glue_7t)

        KEEP(*(.init))
        KEEP(*(.fini))

        . = ALIGN(4);

        _etext = .;
    } > FLASH


    /*
     * Read-only data
     */
    .rodata :
    {
        . = ALIGN(4);

        *(.rodata)
        *(.rodata*)

        . = ALIGN(4);
    } > FLASH


    /*
     * ARM exception handling information
     */
    .ARM_extab :
    {
        . = ALIGN(4);

        *(.ARM.extab* .gnu.linkonce.armextab.*)

        . = ALIGN(4);
    } > FLASH


    .ARM :
    {
        . = ALIGN(4);

        __exidx_start = .;

        *(.ARM.exidx*)

        __exidx_end = .;

        . = ALIGN(4);
    } > FLASH


    /*
     * Initialized data
     *
     * Stored in FLASH but copied to RAM at startup.
     */
    _sidata = LOADADDR(.data);

    .data :
    {
        . = ALIGN(4);

        _sdata = .;

        *(.data)
        *(.data*)

        . = ALIGN(4);

        _edata = .;

    } > RAM AT > FLASH


    /*
     * Uninitialized data
     *
     * Cleared to zero during startup.
     */
    .bss :
    {
        . = ALIGN(4);

        _sbss = .;

        __bss_start__ = _sbss;

        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);

        _ebss = .;

        __bss_end__ = _ebss;

    } > RAM


    /*
     * Heap and Stack
     */
    ._user_heap_stack :
    {
        . = ALIGN(8);

        PROVIDE(end = .);
        PROVIDE(_end = .);

        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;

        . = ALIGN(8);

    } > RAM


    /*
     * Remove unused standard library sections.
     */
    /DISCARD/ :
    {
        libc.a(*)
        libm.a(*)
        libgcc.a(*)
    }


    /*
     * ARM attributes
     */
    .ARM.attributes 0 :
    {
        *(.ARM.attributes)
    }
}
