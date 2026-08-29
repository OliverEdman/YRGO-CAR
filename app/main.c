/**
 * Test kod för att testa startup.c, linker script samt GPIO och systick driver.
 * Just nu blinkar vi en led på a0.
 */


#include "stm32f446.h"
#include "gpio.h"
#include "systick.h"

#include <stddef.h>

#define SYSTEM_CLOCK_HZ 16000000UL
#define LED_PIN 0U // a0

int main(void)
{

    systick_init(SYSTEM_CLOCK_HZ);

    struct gpio *led = gpio_new(GPIOA, LED_PIN, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, 0);
    if (led == NULL) {
        while (1); 
    }

    uint32_t last_toggle = systick_get_ticks();

    while (1) {
        if ((systick_get_ticks() - last_toggle) >= 100U) {
            gpio_toggle(led);
            last_toggle = systick_get_ticks();
        }
    }

    return 0;
}