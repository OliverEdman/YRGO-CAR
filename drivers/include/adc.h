/**
 *
 * @brief Interface for the ADC driver.
 */

#pragma once

#include <stdint.h>

/**
 * @brief Initialize the ADC peripheral (set voltage reference and prescaler).
 */

void adc_init(void);

/**
 * @brief Read the raw digital value (0-1023) from a specific ADC channel.
 *
 * @param channel The ADC channel to read from (0-7).
 *
 * @return 10-bit raw ADC value.
 */
uint16_t adc_read_raw(uint8_t channel);

/**
 * @brief Read the analog voltage (0.0V - 5.0V) from a specific ADC channel.
 *
 * @param channel The ADC channel to read from (0-7).
 *
 * @return Converted voltage as a float.
 */
float adc_read_voltage(uint8_t channel);