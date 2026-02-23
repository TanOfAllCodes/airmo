/*
 * STM32H753ZI Linker Script
 * Cortex-M7 — 2MB Flash, 1MB RAM (DTCM + AXI SRAM + SRAM1-4)
 *
 * Simplified layout: uses DTCM for stack and AXI SRAM for heap/data.
 */

MEMORY
{
    FLASH  (rx)  : ORIGIN = 0x08000000, LENGTH = 2048K
    DTCM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K    /* fast, tightly-coupled */
    AXISRAM(rwx) : ORIGIN = 0x24000000, LENGTH = 512K    /* main application RAM  */
}

ENTRY(Reset_Handler)

SECTIONS
{
    .text :
    {
        *(.isr_vector)
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    _sidata = LOADADDR(.data);

    .data :
    {
        _sdata = .;
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        _edata = .;
    } > AXISRAM AT > FLASH

    .bss :
    {
        _sbss = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        end = .;
        __end__ = .;
    } > AXISRAM

    /* Stack lives at the top of DTCM for fast access */
    _estack = ORIGIN(DTCM) + LENGTH(DTCM);
}
