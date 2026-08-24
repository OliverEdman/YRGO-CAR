#include "stm32f446.h"
#include "gpio.h"
#include "utils.h"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

struct gpio {
	GPIO_TypeDef *port; // pekare till GPIO peripheral port.
	uint8_t pin;	    // pin number 0-15.
	bool in_use;	    // flagga för hålla koll på arrayen
};

static struct gpio gpio_pool[GPIO_MAX_INSTANCES];

struct gpio *gpio_new(void *port, uint8_t pin, uint32_t mode)
{
	if (port == NULL || pin > 15U)
		return NULL;

	GPIO_TypeDef *gpio_port = (GPIO_TypeDef *)port;

	gpio_port->MODER &= ~(0x3UL << (pin * 2U));
	gpio_port->MODER |= (mode << (pin * 2U));
	gpio_port->PUPDR &= ~(0x3UL << (pin * 2U));

	/* Hitta en ledig plats i arrayen*/
	for (uint8_t i = 0; i < GPIO_MAX_INSTANCES; i++) {
		if (!gpio_pool[i].in_use) {
			gpio_pool[i].port = gpio_port;
			gpio_pool[i].pin = pin;
			gpio_pool[i].in_use = true;
			return &gpio_pool[i];
		}
	}

	return NULL; // Om arrayen är full retunera NULL.
}

void gpio_delete(struct gpio **self)
{
	if (self == NULL || *self == NULL)
		return;

	(*self)->in_use = false;
	*self = NULL;
}

uint8_t gpio_write(struct gpio *self, bool state)
{
	if (self == NULL || self->port == NULL)
		return 1; // Error code

	if (state) {
		SET(self->port->ODR, self->pin);
	} else {
		CLEAR(self->port->ODR, self->pin);
	}

	return 0;
}

bool gpio_read(const struct gpio *self)
{
	if (self == NULL || self->port == NULL) {
		return false;
	}

	return READ(self->port->IDR, self->pin);
}

uint8_t gpio_toggle(struct gpio *self)
{
	if (self == NULL || self->port == NULL)
		return 1; // Error code

	TOGGLE(self->port->ODR, self->pin);

	return 0;
}