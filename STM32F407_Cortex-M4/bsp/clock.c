#include "clock.h"

/*
 * Current system clock frequency.
 */
static uint32_t system_clock_frequency =
    SYSTEM_CLOCK_HZ;

/*
 * Initialize system clock.
 */
void Clock_Init(void)
{
    /*
     * STM32F407 clock configuration
     *
     * HSE / PLL
     *     ↓
     * PLL
     *     ↓
     * SYSCLK
     *
     * Target:
     * 168 MHz
     *
     * In a complete project this function
     * configures RCC registers or calls the
     * STM32 HAL RCC configuration.
     */

    system_clock_frequency = SYSTEM_CLOCK_HZ;
}

/*
 * Get system clock frequency.
 */
uint32_t Clock_GetFrequency(void)
{
    return system_clock_frequency;
}
