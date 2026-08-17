#ifndef MAIN_H
#define MAIN_H

/*
 * ============================================================
 * STM32F407 Cortex-M4
 * Scrambler Word Solver
 *
 * Main Application Header
 * ============================================================
 */

#include <stdint.h>
#include <stddef.h>

/*----------------------------------------------------------
 * Project Configuration
 *----------------------------------------------------------*/

#include "config.h"

/*----------------------------------------------------------
 * Common Types
 *----------------------------------------------------------*/

#include "types.h"

/*----------------------------------------------------------
 * Error Handling
 *----------------------------------------------------------*/

#include "error.h"

/*----------------------------------------------------------
 * CMSIS / MCU
 *----------------------------------------------------------*/

#include "stm32f407xx.h"

/*----------------------------------------------------------
 * BSP
 *----------------------------------------------------------*/

#include "board.h"
#include "clock.h"

/*----------------------------------------------------------
 * Drivers
 *----------------------------------------------------------*/

#include "gpio.h"
#include "uart.h"
#include "spi.h"
#include "flash.h"
#include "lcd.h"
#include "timer.h"

/*----------------------------------------------------------
 * Application
 *----------------------------------------------------------*/

#include "scrambler.h"
#include "input_manager.h"
#include "dictionary.h"
#include "result_manager.h"


/*----------------------------------------------------------
 * Application Version
 *----------------------------------------------------------*/

#define PROJECT_NAME \
    "STM32F407 Scrambler Word Solver"

#define PROJECT_VERSION_MAJOR       1U
#define PROJECT_VERSION_MINOR       0U
#define PROJECT_VERSION_PATCH       0U


/*----------------------------------------------------------
 * Main Application
 *----------------------------------------------------------*/

/*
 * Initialize complete application.
 */
status_t Application_Init(void);

/*
 * Main application processing.
 */
void Application_Run(void);

/*
 * Application shutdown.
 */
void Application_DeInit(void);


/*----------------------------------------------------------
 * System Initialization
 *----------------------------------------------------------*/

/*
 * Initialize MCU and board.
 */
status_t System_Init(void);

/*
 * Initialize all peripherals.
 */
status_t Peripheral_Init(void);


/*----------------------------------------------------------
 * Debug
 *----------------------------------------------------------*/

#if CONFIG_DEBUG_ENABLE

#define DEBUG_PRINT(message) \
    UART_SendString(message)

#define DEBUG_PRINT_LINE(message) \
    UART_SendLine(message)

#else

#define DEBUG_PRINT(message) \
    do { } while (0)

#define DEBUG_PRINT_LINE(message) \
    do { } while (0)

#endif


/*----------------------------------------------------------
 * Main Entry
 *----------------------------------------------------------*/

int main(void);


#endif /* MAIN_H */
