/**
 *
 * @brief Interface for the PWM driver.
 *
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

struct pwm;

struct pwm *pwm_init(uint8_t pin, uint8_t frequency);

bool pwm_start(struct pwm *self);

bool pwm_stop(struct pwm *self);

bool pwm_set_duty_cycle(struct pwm *self, uint8_t duty_cycle);