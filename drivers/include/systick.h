/**
 * @brief SysTick timer driver for ARM Cortex-M4 (STM32F446).
 *
 * Provides non-blocking system tick accounting and millisecond delay 
 * functionality using the dedicated core SysTick timer.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initializes the ARM Cortex-M4 SysTick timer.
 *
 * Configures the reload register based on the system clock frequency to 
 * generate an interrupt every 1 millisecond.
 *
 * @param sys_clk_hz Core clock frequency in Hz (Example: 16000000UL for 16 MHz).
 */
bool systick_init(uint32_t sys_clk_hz);

/**
 * @brief Gets the elapsed time since system boot.
 *
 * @return Current tick count in milliseconds.
 */
uint32_t systick_get_ticks(void);

/**
 * @brief Interrupt Service Routine (ISR) for the SysTick timer.
 *
 * Automatically triggered every 1 ms by hardware. Increments the internal 
 * millisecond tick counter ++.
 */
void SysTick_Handler(void);
