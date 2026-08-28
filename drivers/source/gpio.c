/**
 * @file gpio.c
 * @brief GPIO driver implementation for STM32F446
 */

#include "stm32f446.h"
#include "gpio.h"

#include <stddef.h>

struct gpio {
    struct gpio_registers *port;
    uint8_t pin;
    bool in_use;
};

static struct gpio gpio_pool[GPIO_MAX_INSTANCES];

/**
 * Helper: Aktiverar klockan i RCC för rätt GPIO-port om den inte redan är igång.
 */
static void gpio_enable_clock(const struct gpio_registers *port)
{
    if (port == GPIOA) {
        RCC->AHB1ENR |= (1U << 0);
    } else if (port == GPIOB) {
        RCC->AHB1ENR |= (1U << 1);
    } else if (port == GPIOC) {
        RCC->AHB1ENR |= (1U << 2);
    } else if (port == GPIOD) {
        RCC->AHB1ENR |= (1U << 3);
    } else if (port == GPIOE) {
        RCC->AHB1ENR |= (1U << 4);
    } else if (port == GPIOF) {
        RCC->AHB1ENR |= (1U << 5);
    } else if (port == GPIOG) {
        RCC->AHB1ENR |= (1U << 6);
    } else if (port == GPIOH) {
        RCC->AHB1ENR |= (1U << 7);
    }
}

struct gpio *gpio_new(void *port, uint8_t pin, uint32_t mode, uint32_t pupd, uint8_t af)
{
    if (port == NULL || pin > 15U) {
        return NULL;
    }

    /* Hitta en ledig instans i poolen */
    struct gpio *self = NULL;
    for (size_t i = 0; i < GPIO_MAX_INSTANCES; i++) {
        if (!gpio_pool[i].in_use) {
            self = &gpio_pool[i];
            break;
        }
    }

    if (self == NULL) {
        return NULL; /* Inga lediga instanser kvar */
    }

    self->port = (struct gpio_registers *)port;
    self->pin = pin;
    self->in_use = true;

    /* 1. Aktivera peripheralklockan i RCC */
    gpio_enable_clock(self->port);

    /* 2. Konfigurera Mode (MODER: 2 bitar per pinn) */
    self->port->MODER &= ~(0x3UL << (pin * 2U));
    self->port->MODER |= ((mode & 0x3UL) << (pin * 2U));

    /* 3. Konfigurera Pull-up/Pull-down (PUPDR: 2 bitar per pinn) */
    self->port->PUPDR &= ~(0x3UL << (pin * 2U));
    self->port->PUPDR |= ((pupd & 0x3UL) << (pin * 2U));

    /* 4. Konfigurera Alternate Function om mode == GPIO_MODE_ALT */
    if ((mode & 0x3UL) == GPIO_MODE_ALT) {
        uint8_t af_index = pin / 8U;      /* 0 för pin 0-7 (AFRL), 1 för pin 8-15 (AFRH) */
        uint8_t af_shift = (pin % 8U) * 4U; /* 4 bitar per pinn */

        self->port->AFR[af_index] &= ~(0xFUL << af_shift);
        self->port->AFR[af_index] |= (((uint32_t)af & 0xFUL) << af_shift);
    }

    return self;
}

void gpio_delete(struct gpio **self)
{
    if (self == NULL || *self == NULL) {
        return;
    }

    /* Återställ pinnen till standardläge (Input, no pull) */
    (*self)->port->MODER &= ~(0x3UL << ((*self)->pin * 2U));
    (*self)->port->PUPDR &= ~(0x3UL << ((*self)->pin * 2U));

    (*self)->in_use = false;
    *self = NULL;
}

uint8_t gpio_write(struct gpio *self, bool state)
{
    if (self == NULL || !self->in_use) {
        return 1;
    }

    /* BSRR har Set i låga 16 bitarna och Reset i höga 16 bitarna */
    if (state) {
        self->port->BSRR = (1U << self->pin);
    } else {
        self->port->BSRR = (1U << (self->pin + 16U));
    }

    return 0;
}

bool gpio_read(const struct gpio *self)
{
    if (self == NULL || !self->in_use) {
        return false;
    }

    return (self->port->IDR & (1U << self->pin)) != 0U;
}

uint8_t gpio_toggle(struct gpio *self)
{
    if (self == NULL || !self->in_use) {
        return 1;
    }

    /* Läs aktuellt läge från ODR och vippa pinnen med BSRR */
    bool current_state = (self->port->ODR & (1U << self->pin)) != 0U;
    return gpio_write(self, !current_state);
}