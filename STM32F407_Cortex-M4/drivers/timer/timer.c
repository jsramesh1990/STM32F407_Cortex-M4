#include "timer.h"

#include "stm32f407xx.h"

/*
 * TIM2 is connected to APB1.
 *
 * The timer is configured to generate:
 *
 *     1 tick = 1 microsecond
 *
 * Therefore:
 *
 *     1 MHz timer frequency
 *
 * This makes the counter directly represent
 * microseconds.
 */

/*----------------------------------------------------------
 * Configuration
 *----------------------------------------------------------*/

#define TIMER_INPUT_CLOCK_HZ      84000000UL

#define TIMER_FREQUENCY_HZ        1000000UL

#define TIMER_PRESCALER           \
    ((TIMER_INPUT_CLOCK_HZ / TIMER_FREQUENCY_HZ) - 1U)

/*
 * TIM2 is a 32-bit timer.
 *
 * It will overflow after approximately:
 *
 *     4294 seconds
 *
 * at 1 MHz.
 */

/*----------------------------------------------------------
 * Timer_Init
 *----------------------------------------------------------*/

TimerStatus_t Timer_Init(void)
{
    /*
     * Enable TIM2 peripheral clock.
     *
     * TIM2 is connected to APB1.
     */

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    /*
     * Reset timer configuration.
     */

    TIM2->CR1 = 0U;
    TIM2->CR2 = 0U;
    TIM2->SMCR = 0U;
    TIM2->DIER = 0U;

    /*
     * Configure prescaler.
     *
     * 84 MHz / 84 = 1 MHz
     *
     * Therefore:
     *
     * 1 timer tick = 1 us
     */

    TIM2->PSC = TIMER_PRESCALER;

    /*
     * Maximum auto-reload value.
     */

    TIM2->ARR = 0xFFFFFFFFUL;

    /*
     * Start counter from zero.
     */

    TIM2->CNT = 0U;

    /*
     * Generate update event so that
     * prescaler value is loaded.
     */

    TIM2->EGR = TIM_EGR_UG;

    /*
     * Clear update flag.
     */

    TIM2->SR &= ~TIM_SR_UIF;

    /*
     * Enable counter.
     */

    TIM2->CR1 |= TIM_CR1_CEN;

    return TIMER_OK;
}

/*----------------------------------------------------------
 * Timer_DeInit
 *----------------------------------------------------------*/

void Timer_DeInit(void)
{
    /*
     * Disable TIM2.
     */

    TIM2->CR1 &= ~TIM_CR1_CEN;

    /*
     * Reset TIM2 peripheral.
     */

    RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST;

    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;
}

/*----------------------------------------------------------
 * Timer_GetMicros
 *----------------------------------------------------------*/

uint32_t Timer_GetMicros(void)
{
    return TIM2->CNT;
}

/*----------------------------------------------------------
 * Timer_GetMillis
 *----------------------------------------------------------*/

uint32_t Timer_GetMillis(void)
{
    /*
     * Timer runs at 1 MHz.
     *
     * 1000 us = 1 ms
     */

    return TIM2->CNT / 1000U;
}

/*----------------------------------------------------------
 * Timer_DelayUs
 *----------------------------------------------------------*/

void Timer_DelayUs(uint32_t microseconds)
{
    uint32_t start;

    start = Timer_GetMicros();

    /*
     * Unsigned subtraction naturally handles
     * timer counter overflow.
     */

    while ((uint32_t)(
        Timer_GetMicros() - start) < microseconds)
    {
        /*
         * Wait.
         */
    }
}

/*----------------------------------------------------------
 * Timer_DelayMs
 *----------------------------------------------------------*/

void Timer_DelayMs(uint32_t milliseconds)
{
    uint32_t start;

    start = Timer_GetMillis();

    while ((uint32_t)(
        Timer_GetMillis() - start) < milliseconds)
    {
        /*
         * Wait.
         */
    }
}

/*----------------------------------------------------------
 * Timer_StartTimeout
 *----------------------------------------------------------*/

uint32_t Timer_StartTimeout(void)
{
    return Timer_GetMicros();
}

/*----------------------------------------------------------
 * Timer_TimeoutExpired
 *----------------------------------------------------------*/

uint8_t Timer_TimeoutExpired(uint32_t start_time,
                             uint32_t timeout_us)
{
    uint32_t elapsed;

    elapsed =
        Timer_GetMicros() - start_time;

    if (elapsed >= timeout_us)
    {
        return 1U;
    }

    return 0U;
}

/*----------------------------------------------------------
 * Timer_GetTimestamp
 *----------------------------------------------------------*/

uint32_t Timer_GetTimestamp(void)
{
    return Timer_GetMicros();
}

/*----------------------------------------------------------
 * Timer_GetElapsedUs
 *----------------------------------------------------------*/

uint32_t Timer_GetElapsedUs(uint32_t start_time)
{
    return Timer_GetMicros() - start_time;
}
