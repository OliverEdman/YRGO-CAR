/*
 * ============================================================================
 * @file startup.s
 * @brief Uppstartssekvens och vektortabell för STM32F446 (ARM Cortex-M4).
 * ============================================================================
 */

/*
 * ============================================================================
 * ASSEMBLERDIREKTIV OCH SYMBOLER
 * ============================================================================
 */
.syntax unified
.cpu cortex-m4
.thumb

/* Exportera symboler så att länkaren kan komma åt dem */
.global Reset_Handler
.global Vector_table
.global Default_Handler

/* Importera externa symboler från C-koden och linker scriptet (.ld) */
.extern main                 @ Main-funktionen
.extern _estack              @ Minnesadressen till toppen av RAM (Stack Pointer)
.extern _sidata              @ Minnesadress i FLASH där initialvärden för .data ligger
.extern _sdata               @ Startadress i RAM för .data-sektionen
.extern _edata               @ Slutadress i RAM för .data-sektionen
.extern _sbss                @ Startadress i RAM för .bss-sektionen
.extern _ebss                @ Slutadress i RAM för .bss-sektionen

/*
 * ============================================================================
 * VEKTORTABELL (ISR Vector Table)
 * Placeras först i FLASH (.isr_vector) enligt inställningarna i linker scriptet.
 * ============================================================================
 */
.section .isr_vector, "a", %progbits 
.type Vector_table, %object

Vector_table: 
    .word _estack            @ 0: Adress till vart stacken börjar (Stack Pointer - SP)
    .word Reset_Handler      @ 1: Adress till funktionen Reset Handler (Körs vid reset)
    .word NMI_Handler        @ 2: Non Maskable Interrupt
    .word HardFault_Handler  @ 3: Hard Fault
    .word MemManage_Handler  @ 4: Memory Management Fault
    .word BusFault_Handler   @ 5: Bus Fault (Bussfel vid minnesläsning/skrivning)
    .word UsageFault_Handler @ 6: Usage Fault (Ogiltig instruktion eller tillstånd)
    .word 0, 0, 0, 0         @ 7-10: Reserverade platser
    .word SVC_Handler        @ 11: System Service Call (OS-anrop via supervisor)
    .word DebugMon_Handler   @ 12: Debug Monitor
    .word 0                  @ 13: Reserverad plats
    .word PendSV_Handler     @ 14: Pendable System Call
    .word SysTick_Handler    @ 15: SysTick Timer Interrupt Handler

/*
 * ============================================================================
 * KÖRBAR PROGRAMKOD (.text)
 * Processorns första instruktioner efter Reset.
 * ============================================================================
 */
.section .text

.thumb_func
.type Reset_Handler, %function
Reset_Handler:

/*
 * ============================================================================
 * DENNA KOD AKTIVERAR FPU (FLOATING-POINT-UNIT)
 * Sätter bitarna 20-23 i CPACR-registret (0xE000ED88) till 1111 (Full Access).
 * ============================================================================
 */
    LDR     r0, =0xE000ED88
    LDR     r1, [r0]
    ORR     r1, r1, #0xF00000
    STR     r1, [r0]
    DSB
    ISB

/*
 * ============================================================================
 * DENNA KOD KOPIERAR STARTVÄRDEN PÅ VARIABLER OCH SKICKAR TILL RAM (FLASH -> RAM)
 * Kopierar initierade globala/statiska variabler från FLASH till SRAM. 
 * ============================================================================
 */
    LDR     r0, =_sdata      @ r0 = Startadress för var variabler ska ligga i RAM
    LDR     r1, =_edata      @ r1 = Slutadress för var variabler ska ligga i RAM
    LDR     r2, =_sidata     @ r2 = Minnesadress för var globala variabler ska liga i FLASH

copy_data_loop:
    CMP     r0, r1
    BGE     init_bss

    LDR     r3, [r2], #4
    STR     r3, [r0], #4

    B       copy_data_loop

/*
 * ============================================================================
 * NOLLSTÄLL .BSS-SEKTIONEN i (RAM).
 * Nollställer alla oinitierade globala/statiska variabler i RAM.
 * ============================================================================
 */
init_bss:
    LDR     r0, =_sbss
    LDR     r1, =_ebss
    MOVS    r2, #0

zero_bss_loop:
    CMP     r0, r1
    BGE     call_main

    STR     r2, [r0], #4

    B       zero_bss_loop
/*
 * ============================================================================
 * HOPPA TILL C-PROGRAMMET (main)
 * ============================================================================
 */
call_main:
    BL      main             @ Anropa C-funktionen main()

/* Om vårat main program retunerar 0 hamnar vi här och fastar i en oädnlig loop.*/
Default_Loop:
    B       Default_Loop

/*
 * ============================================================================
 * DEFAULT HANDLER
 * Stanna kvar i en oändlig loop vid fel/ohanterat avbrott
 * ============================================================================
 */
.thumb_func
.type Default_Handler, %function
Default_Handler:
    B       Default_Handler  @ Stanna kvar i en oändlig loop vid fel/ohanterat avbrott