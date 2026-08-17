#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/*
 * STM32F407 Cortex-M4 Timer Driver
 *
 * Uses:
 *     TIM2
 *
 * TIM2 is a 32-bit general-purpose timer.
 *
 * Main uses:
 *     - Millisecond delay
 *     - Microsecond delay
 *     - Timeout handling
 *     - Elapsed time measurement
 */

/*----------------------------------------------------------
 * Timer Status
 *----------------------------------------------------------*/

typedef enum
{
    TIMER_OK = 0,
    TIMER_ERROR,
    TIMER_INVALID_PARAMETER

} TimerStatus_t;

/*----------------------------------------------------------
 * Timer Initialization
 *----------------------------------------------------------*/

/*
 * Initialize TIM2 as a free-running microsecond timer.
 */
TimerStatus_t Timer_Init(void);

/*
 * Stop/deinitialize TIM2.
 */
void Timer_DeInit(void);

/*----------------------------------------------------------
 * Delay Functions
 *----------------------------------------------------------*/

/*
 * Delay for milliseconds.
 */
void Timer_DelayMs(uint32_t milliseconds);

/*
 * Delay for microseconds.
 */
void Timer_DelayUs(uint32_t microseconds);

/*----------------------------------------------------------
 * Time Functions
 *----------------------------------------------------------*/

/*
 * Return current timer counter in microseconds.
 */
uint32_t Timer_GetMicros(void);

/*
 * Return current timer counter in milliseconds.
 */
uint32_t Timer_GetMillis(void);

/*----------------------------------------------------------
 * Timeout Functions
 *----------------------------------------------------------*/

/*
 * Start a timeout counter.
 */
uint32_t Timer_StartTimeout(void);

/*
 * Check whether timeout has expired.
 *
 * Returns:
 *
 *     0 -> timeout not expired
 *     1 -> timeout expired
 */
uint8_t Timer_TimeoutExpired(uint32_t start_time,
                             uint32_t timeout_us);

/*----------------------------------------------------------
 * Performance Measurement
 *----------------------------------------------------------*/

/*
 * Capture current timestamp.
 */
uint32_t Timer_GetTimestamp(void);

/*
 * Calculate elapsed time.
 *
 * Returns elapsed microseconds.
 */
uint32_t Timer_GetElapsedUs(uint32_t start_time);

#endif /* TIMER_H */
