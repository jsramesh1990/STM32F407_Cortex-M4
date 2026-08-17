#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

/*
 * Board initialization
 */
void Board_Init(void);

/*
 * Board peripheral initialization
 */
void Board_Peripherals_Init(void);

/*
 * LED control
 */
void Board_LED_On(void);
void Board_LED_Off(void);
void Board_LED_Toggle(void);

/*
 * Board delay
 */
void Board_Delay(uint32_t milliseconds);

#endif
