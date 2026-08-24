#include <stdint.h>

/*
 * "extern" means these symbols are defined outside this file (in stm32f446.ld)
 */
extern int main(void);
void Reset_Handler(void);

/* Import the exact memory boundary addresses from the Linker Script */
extern uint32_t _estack; /* Top of RAM, where the stack begins */
extern uint32_t _sidata; /* Start of .data initialization values in FLASH */
extern uint32_t _sdata;	 /* Start of .data section in RAM */
extern uint32_t _edata;	 /* End of .data section in RAM */
extern uint32_t _sbss;	 /* Start of .bss section in RAM */
extern uint32_t _ebss;	 /* End of .bss section in RAM */

/* Struct that matches exactly what the Cortex-M4 hardware expects at boot */
typedef struct {
	uint32_t *initial_sp_value;  /* Address for Stack Pointer (SP) */
	void (*reset_handler)(void); /* Address to the first instruction (PC) */
} VectorTable;

/**
 * __attribute__((section(".isr_vector")))
 * We force the compiler to place this specific structure at the very beginning
 * of the FLASH memory (0x08000000) so the CPU finds it immediately at power-on.
 */
__attribute__((section(".isr_vector"))) const VectorTable vector_table = {
	.initial_sp_value =
		&_estack, /* Step 1: Give the processor its workspace (Stack) */
	.reset_handler =
		Reset_Handler /* Step 2: Tell it where the execution starts */
};

/**
 * This is the very first function that runs in the kernel.
 * It functions as a bridge between the raw hardware and the C environment.
 */
void Reset_Handler(void)
{
	/* Copy initialized global variables (.data) from FLASH to RAM */
	uint32_t *src = &_sidata;
	uint32_t *dst = &_sdata;
	while (dst < &_edata) {
		*dst++ = *src++;
	}

	/* Clear uninitialized global variables (.bss) in RAM to zero */
	dst = &_sbss;
	while (dst < &_ebss) {
		*dst++ = 0;
	}

	/* The C environment is now 100% ready. Jump to your main application!
	 */
	main();

	/* If main, should return, trap the CPU here forever */
	while (1)
		;
}
