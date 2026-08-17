#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

/*
 * STM32F407 system clock
 */
#define SYSTEM_CLOCK_HZ    168000000UL

/*
 * Initialize system clock.
 */
void Clock_Init(void);

/*
 * Return system clock frequency.
 */
uint32_t Clock_GetFrequency(void);

#endif
