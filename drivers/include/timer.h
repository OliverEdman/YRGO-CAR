/**
 * @file timer.h
 * @brief Interface for a software timer using a static pool.
 *
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Forward declaration of the opaque timer structure.
 *
 * This keeps the structure details hidden inside the .c file so
 * that code outside the driver cannot change its data directly.
 */
struct timer;

/**
 * @brief Timer expiration callback function.
 */
typedef void (*timer_callback_t)(void);

/**
 * @brief Initialize the hardware timer and the static timer pool.
 *
 * Configures Timer0 on the ATmega328P to CTC mode with a prescaler
 * of 64 to create a system tick every 1 millisecond.
 */
void timer_init(void);

/**
 * @brief Background handler that updates and runs active timers.
 *
 * This function checks the time and runs the callbacks when a timer finishes.
 * It must be called continuously inside the main loop's while(1).
 */
void timer_handler(void);

/**
 * @brief Request a software timer slot from the static timer pool.
 *
 * @param[in] timeout_ms How long the timer should count in milliseconds.
 * @param[in] callback The function to run when the timer finishes.
 *
 * @note This uses a fixed size array in RAM instead of malloc() to avoid
 * memory fragmentation, making it safe and predictable.
 *
 * @return Pointer to the new timer instance, or NULL if the pool is full.
 */
struct timer *timer_new(uint32_t timeout_ms, timer_callback_t callback);

/**
 * @brief Delete a software timer and gives its slot back to the pool.
 *
 * Stop the timer, frees its spot in the array, and sets the caller's
 * pointer to NULL so it cannot be accidentally used again.
 *
 * @param[in,out] self Double pointer to the timer instance to delete.
 */
void timer_delete(struct timer **self);

/**
 * @brief Check if the timer is currently running.
 *
 * @param[in] self Pointer to the timer instance.
 * @return true if the timer is actively counting, false otherwise.
 */
bool timer_is_enabled(const struct timer *self);

/**
 * @brief Start the software timer.
 *
 * @param[in] self Pointer to the timer instance.
 */
void timer_start(struct timer *self);

/**
 * @brief Stop the software timer without resetting or deleting it.
 *
 * @param[in] self Pointer to the timer instance.
 */
void timer_stop(struct timer *self);

/**
 * @brief Toggle the timer between started and stopped states.
 *
 * @param[in] self Pointer to the timer instance.
 */
void timer_toggle(struct timer *self);

/**
 * @brief Reset the timer's counter back to zero and starts it.
 *
 * @param[in] self Pointer to the timer instance.
 */
void timer_restart(struct timer *self);

/**
 * @brief Reset the timer's counter back to zero and stops it.
 *
 * @param[in] self Pointer to the timer instance.
 */
void timer_reset(struct timer *self);

/**
 * @brief Get the timer's set time.
 *
 * This function asks the timer: "How many milliseconds are you set to count?"
 * and returns the answer.
 *
 * @param[in] self The timer you want to check.
 * @return The set time in milliseconds, or 0 if the timer does not exist.
 */
uint32_t timer_timeout_ms(const struct timer *self);

/**
 * @brief Change the timeout time of an existing timer and resets its counter.
 *
 * @param[in] self       Pointer to the timer instance.
 * @param[in] timeout_ms The new target time in milliseconds. Must be greater
 * than 0.
 */
void timer_set_timeout_ms(struct timer *self, uint32_t timeout_ms);