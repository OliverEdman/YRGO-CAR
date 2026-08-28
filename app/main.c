#include "stm32f446.h"
#include "systick.h"

int main(void)
{
    systick_init(16000000UL);

    RCC->AHB1ENR |= (1U << 0);
    (void)RCC->AHB1ENR; 

    GPIOA->MODER &= ~(0x3UL << (0 * 2));
    GPIOA->MODER |=  (0x1UL << (0 * 2));


    uint32_t last_toggle = systick_get_ticks();

    while (1) {
        if ((systick_get_ticks() - last_toggle) >= 100U) {
            GPIOA->ODR ^= (1U << 0);
            last_toggle = systick_get_ticks();
        }
    }

    return 0;
}