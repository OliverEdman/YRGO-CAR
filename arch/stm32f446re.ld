/*
 * This is the map of the processor. It tells the linker
 * exactly where the physical memory chips are located and how much space is available.
 */

/* Define the physical memory regions in the chip */

MEMORY
{
    /* FLASH: This is where the code is saved permanently. 
       (rx) means Read and Execute (i can read and run code from here).
       ORIGIN is the start address in the chip, LENGTH is the size (512 KB). */
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K

    /* RAM: This is where variables and the stack live when the program is running.
       (xrw) means that i can also write (Write) to this area.
       ORIGIN is the start address for SRAM, LENGTH is the size (128 KB). */
    RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Define how the sections from the C code should be placed in memory */

SECTIONS
{
    /* Calculate the top of RAM dynamically where the stack begins.
       0x20000000 (ORIGIN) + 128K (LENGTH) = 0x20020000. 
       This symbol is exported to startup.c. */
    _estack = ORIGIN(RAM) + LENGTH(RAM);

    /**
     * .isr_vector: This is the most important section.
     * It contains the Vector Table (Stack-top and Reset_Handler).
     * It MUST be located first in Flash for the processor to be able to start according to datasheet.
     */
    .isr_vector :
    {
        . = ALIGN(4);        /* Ensure the next bit of data lands on an even 32-bit address */
        KEEP(*(.isr_vector)) /* Tell the linker: "Do not throw this away even if no one calls it!" */
        . = ALIGN(4);
    } > FLASH

    /**
     * .text: All program code ends up here.
     * All functions (Reset_Handler, main, etc.) are placed here.
     */
    .text :
    {
        . = ALIGN(4);
        *(.text)             /* Take all code from all your compiled .c files */
        *(.text*)            /* Also include any sub-sections of code */
        . = ALIGN(4);
    } > FLASH

    /**
     * .rodata: Read-Only Data.
     * This is where constant variables (const int) and hardcoded text strings live.
     * These stay permanently in FLASH to save RAM.
     */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    /* This variable stores the exact address in FLASH where the initial values 
       for our global variables are hidden. Used by the startup code. */
    _sidata = LOADADDR(.data);

    /**
     * .data: Initialized Global Data.
     * This is for variables that have a starting value, like "int boot_count = 1;".
     * They are stored in FLASH but must be copied into RAM during startup to be modified.
     * "AT > FLASH" tells the linker to store it in FLASH but calculate addresses for RAM.
     */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;          /* Mark the start of .data in RAM */
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;          /* Mark the end of .data in RAM */
    } > RAM AT > FLASH

    /**
     * .bss: Uninitialized Global Data.
     * This is for global variables that don't have a starting value, like "int error_flag;".
     * They take 0 bytes of space in FLASH. The startup code will just clear this RAM area to zero.
     */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;          /* Mark the start of .bss in RAM */
        *(.bss)
        *(.bss*)
        *(COMMON)            /* Important fallback for some C compilers */
        . = ALIGN(4);
        _ebss = .;          /* Mark the end of .bss in RAM */
    } > RAM
}

