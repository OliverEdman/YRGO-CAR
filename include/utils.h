#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Set bit in the given register.
 */
#define SET(reg, bit) ((reg) |= (1UL << (bit)))

/**
 * @brief Clear bit in the given register.
 */
#define CLEAR(reg, bit) ((reg) &= ~(1UL << (bit)))

/**
 * @brief Toggle bit in the given register.
 */
#define TOGGLE(reg, bit) ((reg) ^= (1UL << (bit)))

/**
 * @brief Read bit in the given register.
 */
#define READ(reg, bit) (bool)((reg) & (1UL << (bit)))