#include <stdint.h>

extern int main(void);
void Reset_Handler(void);
void Default_Handler(void);

/* Weak alias so SysTick_Handler in systick.c overrides this */
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

/* Linker script symbols */
extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

/* 
 * Hardware-aligned Vector Table Array.
 * Vector 0 is the initial Stack Pointer (_estack address casted to function pointer).
 * Vector 15 is SysTick.
 */
__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) = {
    (void (*)(void))&_estack, /* 0: Initial Stack Pointer (0x20020000) */
    Reset_Handler,             /* 1: Reset Handler */
    Default_Handler,           /* 2: NMI */
    Default_Handler,           /* 3: HardFault */
    Default_Handler,           /* 4: MemManage */
    Default_Handler,           /* 5: BusFault */
    Default_Handler,           /* 6: UsageFault */
    0, 0, 0, 0,                /* 7-10: Reserved */
    Default_Handler,           /* 11: SVCall */
    Default_Handler,           /* 12: Debug Monitor */
    0,                         /* 13: Reserved */
    Default_Handler,           /* 14: PendSV */
    SysTick_Handler            /* 15: SysTick Handler */
};

void Reset_Handler(void)
{
    /* Copy .data section from FLASH to RAM */
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* Zero-initialize .bss section in RAM */
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    /* Call the application entry point */
    main();

    /* Trap CPU if main returns */
    while (1);
}

void Default_Handler(void)
{
    while (1);
}