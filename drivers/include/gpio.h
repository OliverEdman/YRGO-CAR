#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "stm32f446.h"

#define GPIO_MAX_INSTANCES 16U

/* GPIO Modes (MODER) */
#define GPIO_MODE_INPUT   0x0UL
#define GPIO_MODE_OUTPUT  0x1UL
#define GPIO_MODE_ALT     0x2UL
#define GPIO_MODE_ANALOG  0x3UL

/* GPIO Pull-up / Pull-down (PUPDR) */
#define GPIO_PUPD_NONE    0x0UL
#define GPIO_PUPD_UP      0x1UL
#define GPIO_PUPD_DOWN    0x2UL

/* GPIO Alternate Functions (AFR) */
#define GPIO_AF0          0x0U
#define GPIO_AF1          0x1U  // TIM1/TIM2
#define GPIO_AF2          0x2U  // TIM3/TIM4/TIM5
#define GPIO_AF3          0x3U  // TIM8/TIM9/TIM10/TIM11
#define GPIO_AF4          0x4U  // I2C1/I2C2/I2C3
#define GPIO_AF5          0x5U  // SPI1/SPI2/SPI3/SPI4
#define GPIO_AF6          0x6U  // SPI3/SAI1
#define GPIO_AF7          0x7U  // USART1/USART2/USART3
#define GPIO_AF8          0x8U  // UART4/UART5/USART6
#define GPIO_AF9          0x9U  // CAN1/CAN2/TIM12/TIM13/TIM14
#define GPIO_AF10         0xAU  // OTG_FS/OTG_HS
#define GPIO_AF11         0xBU  // ETH
#define GPIO_AF12         0xCUL // FMC/SDIO/OTG_HS
#define GPIO_AF13         0xDUL // DCMI
#define GPIO_AF14         0xEUL
#define GPIO_AF15         0xFUL // EVENTOUT

struct gpio;

/**
 * gpio_new() - Creates a new GPIO instance and enables port clock.
 * 
 * @port: Pointer to the MCU GPIO peripheral instance (example: GPIOA).
 * @pin: Pin number on the MCU (0-15).
 * @mode: Desired GPIO mode (Use defines, exmaple: GPIO_MODE_INPUT or GPIO_MODE_OUTPUT).
 * @pupd: Pull-up/Pull-down configuration (Use defines, example: GPIO_PUPD_NONE or GPIO_PUPD_UP).
 * @af: Alternate Function mapping (Use defines, example: GPIO_AF1 for Timers. Ignored if mode is not ALT).
 * @return: A pointer to a new allocated &struct gpio instance, or NULL on failure.
 */
struct gpio *gpio_new(void *port, uint8_t pin, uint32_t mode, uint32_t pupd, uint8_t af);

/**
 * gpio_delete() - Frees a GPIO instance and nullifies the pointer.
 * @self: Double pointer to the GPIO object to be freed.
 */
void gpio_delete(struct gpio **self);

/**
 * gpio_write() - Sets the state of a GPIO pin atomically using BSRR.
 * @self: Pointer to the GPIO object.
 * @state: Boolean value (true for high, false for low).
 * @return: 0 on success, 1 on failure.
 */
uint8_t gpio_write(struct gpio *self, bool state);

/**
 * gpio_read() - Reads the state of a GPIO pin.
 * @self: Pointer to the GPIO object.
 * @return: true if the pin is high, false if the pin is low.
 */
bool gpio_read(const struct gpio *self);

/**
 * gpio_toggle() - Toggles the state of a GPIO pin.
 * @self: Pointer to the GPIO object.
 * @return: 0 on success, 1 on failure.
 */
uint8_t gpio_toggle(struct gpio *self);