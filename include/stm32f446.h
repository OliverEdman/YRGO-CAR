/**
 * @file stm32f446.h
 * @brief Memory addresses and peripheral register structures for STM32F446
 */

#pragma once

#include <stdint.h>

/* MEMORY */
#define FLASH_BASE          0x08000000UL
#define SRAM_BASE           0x20000000UL
#define PERIPH_BASE         0x40000000UL

/* BUSES */
#define APB1PERIPH_BASE     PERIPH_BASE
#define APB2PERIPH_BASE     (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE     (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE     0x50000000UL
#define AHB3PERIPH_BASE     0xA0000000UL

/* AHB1 PERIPHERALS */
#define GPIOA_BASE          (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE          (AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE          (AHB1PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE          (AHB1PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE          (AHB1PERIPH_BASE + 0x1000UL)
#define GPIOF_BASE          (AHB1PERIPH_BASE + 0x1400UL)
#define GPIOG_BASE          (AHB1PERIPH_BASE + 0x1800UL)
#define GPIOH_BASE          (AHB1PERIPH_BASE + 0x1C00UL)
#define CRC_BASE            (AHB1PERIPH_BASE + 0x3000UL)
#define RCC_BASE            (AHB1PERIPH_BASE + 0x3800UL)
#define FLASH_R_BASE        (AHB1PERIPH_BASE + 0x3C00UL)
#define DMA1_BASE           (AHB1PERIPH_BASE + 0x6000UL)
#define DMA2_BASE           (AHB1PERIPH_BASE + 0x6400UL)

/* APB1 PERIPHERALS */
#define TIM2_BASE           (APB1PERIPH_BASE + 0x0000UL)
#define TIM3_BASE           (APB1PERIPH_BASE + 0x0400UL)
#define TIM4_BASE           (APB1PERIPH_BASE + 0x0800UL)
#define TIM5_BASE           (APB1PERIPH_BASE + 0x0C00UL)
#define RTC_BASE            (APB1PERIPH_BASE + 0x2800UL)
#define WWDG_BASE           (APB1PERIPH_BASE + 0x2C00UL)
#define IWDG_BASE           (APB1PERIPH_BASE + 0x3000UL)
#define SPI2_BASE           (APB1PERIPH_BASE + 0x3800UL)
#define SPI3_BASE           (APB1PERIPH_BASE + 0x3C00UL)
#define USART2_BASE         (APB1PERIPH_BASE + 0x4400UL)
#define USART3_BASE         (APB1PERIPH_BASE + 0x4800UL)
#define UART4_BASE          (APB1PERIPH_BASE + 0x4C00UL)
#define UART5_BASE          (APB1PERIPH_BASE + 0x5000UL)
#define I2C1_BASE           (APB1PERIPH_BASE + 0x5400UL)
#define I2C2_BASE           (APB1PERIPH_BASE + 0x5800UL)
#define I2C3_BASE           (APB1PERIPH_BASE + 0x5C00UL)
#define PWR_BASE            (APB1PERIPH_BASE + 0x7000UL)
#define DAC_BASE            (APB1PERIPH_BASE + 0x7400UL)

/* APB2 PERIPHERALS */
#define TIM1_BASE           (APB2PERIPH_BASE + 0x0000UL)
#define TIM8_BASE           (APB2PERIPH_BASE + 0x0400UL)
#define USART1_BASE         (APB2PERIPH_BASE + 0x1000UL)
#define USART6_BASE         (APB2PERIPH_BASE + 0x1400UL)
#define ADC_BASE            (APB2PERIPH_BASE + 0x2000UL)
#define SPI1_BASE           (APB2PERIPH_BASE + 0x3000UL)
#define SYSCFG_BASE         (APB2PERIPH_BASE + 0x3800UL)
#define EXTI_BASE           (APB2PERIPH_BASE + 0x3C00UL)

/* CORTEX-M4 INTERNAL PERIPHERALS */
#define SysTick_BASE        (0xE000E010UL)

/* REGISTER STRUCTURES */

struct gpio_registers
{
    volatile uint32_t MODER;   ///< Offset 0x00
    volatile uint32_t OTYPER;  ///< Offset 0x04
    volatile uint32_t OSPEEDR; ///< Offset 0x08
    volatile uint32_t PUPDR;   ///< Offset 0x0C
    volatile uint32_t IDR;     ///< Offset 0x10
    volatile uint32_t ODR;     ///< Offset 0x14
    volatile uint32_t BSRR;    ///< Offset 0x18
    volatile uint32_t LCKR;    ///< Offset 0x1C
    volatile uint32_t AFR[2];  ///< Offset 0x20
};

struct rcc_registers
{
    volatile uint32_t CR;       ///< Offset 0x00
    volatile uint32_t PLLCFGR;  ///< Offset 0x04
    volatile uint32_t CFGR;     ///< Offset 0x08
    volatile uint32_t CIR;      ///< Offset 0x0C
    volatile uint32_t AHB1RSTR; ///< Offset 0x10
    volatile uint32_t AHB2RSTR; ///< Offset 0x14
    volatile uint32_t AHB3RSTR; ///< Offset 0x18
    uint32_t RESERVED0;         ///< Offset 0x1C
    volatile uint32_t APB1RSTR; ///< Offset 0x20
    volatile uint32_t APB2RSTR; ///< Offset 0x24
    uint32_t RESERVED1[2];      ///< Offset 0x28 - 0x2C
    volatile uint32_t AHB1ENR;  ///< Offset 0x30
    volatile uint32_t AHB2ENR;  ///< Offset 0x34
    volatile uint32_t AHB3ENR;  ///< Offset 0x38
    uint32_t RESERVED2;         ///< Offset 0x3C
    volatile uint32_t APB1ENR;  ///< Offset 0x40
    volatile uint32_t APB2ENR;  ///< Offset 0x44
};

struct systick_registers
{
    volatile uint32_t CTRL;  ///< Offset 0x00
    volatile uint32_t LOAD;  ///< Offset 0x04
    volatile uint32_t VAL;   ///< Offset 0x08
    volatile uint32_t CALIB; ///< Offset 0x0C
};

/* PERIPHERAL POINTERS */
#define GPIOA   ((struct gpio_registers *)GPIOA_BASE)
#define GPIOB   ((struct gpio_registers *)GPIOB_BASE)
#define GPIOC   ((struct gpio_registers *)GPIOC_BASE)
#define GPIOD   ((struct gpio_registers *)GPIOD_BASE)
#define GPIOE   ((struct gpio_registers *)GPIOE_BASE)
#define GPIOF   ((struct gpio_registers *)GPIOF_BASE)
#define GPIOG   ((struct gpio_registers *)GPIOG_BASE)
#define GPIOH   ((struct gpio_registers *)GPIOH_BASE)

#define RCC     ((struct rcc_registers *)RCC_BASE)
#define SysTick ((struct systick_registers *)SysTick_BASE)