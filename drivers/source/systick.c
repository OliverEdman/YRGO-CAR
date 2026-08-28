/**
 * @brief SysTick timer driver implementation for STM32F446
 */

#include "systick.h"
#include "stm32f446.h"

#define SYSTICK_CTRL_ENABLE    0U
#define SYSTICK_CTRL_TICKINT   1U
#define SYSTICK_CTRL_CLKSOURCE 2U

/* SysTick is a 24-bit counter (max value 0x00FFFFFF) */
#define SYSTICK_MAX_RELOAD 0x00FFFFFFUL

static volatile uint32_t systick_ms = 0U;

bool systick_init(uint32_t sys_clk_hz)
{
    if (sys_clk_hz == 0U) {
        return false;
    }

    /* Calculate number of clock cycles per millisecond: (sys_clk_hz / 1000) - 1 */
    uint32_t reload_val = (sys_clk_hz / 1000U) - 1U;

    /* Verify that the reload value fits into the 24-bit register */
    if (reload_val > SYSTICK_MAX_RELOAD) {
        return false;
    }

    /* Disable SysTick during configuration */
    SysTick->CTRL = 0U;

    /* Set the reload value for a 1 ms interval */
    SysTick->LOAD = reload_val;

    /* Reset current counter value */
    SysTick->VAL = 0U;

    /* Aktivera AHB-klocka (Bit 2), SysTick-interrupt (Bit 1) och starta timern (Bit 0) i ett svep */
SysTick->CTRL = (1U << SYSTICK_CTRL_CLKSOURCE) | 
                (1U << SYSTICK_CTRL_TICKINT)   | 
                (1U << SYSTICK_CTRL_ENABLE);

    /* Aktivera globala avbrott direkt i drivrutinen */
    __asm__ volatile ("cpsie i" : : : "memory");

    return true;
}

uint32_t systick_get_ticks(void)
{
    return systick_ms;
}

__attribute__((used)) void SysTick_Handler(void)
{
    systick_ms++;
}