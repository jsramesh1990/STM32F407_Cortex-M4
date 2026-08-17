#ifndef UART_H
#define UART_H

#include <stdint.h>

/*
 * STM32F407 USART2 Driver
 *
 * USART2:
 *
 * PA2 -> USART2_TX
 * PA3 -> USART2_RX
 *
 * Default configuration:
 *
 * Baud rate : 115200
 * Data      : 8-bit
 * Parity    : None
 * Stop      : 1
 */

/*----------------------------------------------------------
 * UART Status
 *----------------------------------------------------------*/

typedef enum
{
    UART_OK = 0,
    UART_ERROR,
    UART_TIMEOUT,
    UART_INVALID_PARAMETER

} UART_Status_t;

/*----------------------------------------------------------
 * UART Configuration
 *----------------------------------------------------------*/

typedef struct
{
    uint32_t baud_rate;

} UART_Config_t;

/*----------------------------------------------------------
 * Initialization
 *----------------------------------------------------------*/

/*
 * Initialize USART2 with default configuration.
 *
 * Default baud rate:
 *
 *     115200 baud
 */
UART_Status_t UART_Init(void);

/*
 * Initialize USART2 with custom configuration.
 */
UART_Status_t UART_InitConfig(
    const UART_Config_t *config);

/*
 * Deinitialize USART2.
 */
void UART_DeInit(void);

/*----------------------------------------------------------
 * Transmit
 *----------------------------------------------------------*/

/*
 * Send one character.
 */
void UART_SendChar(char character);

/*
 * Send a null-terminated string.
 */
void UART_SendString(const char *string);

/*
 * Send a string followed by CR/LF.
 */
void UART_SendLine(const char *string);

/*
 * Send unsigned integer.
 */
void UART_SendUInt(uint32_t value);

/*
 * Send signed integer.
 */
void UART_SendInt(int32_t value);

/*
 * Send a hexadecimal value.
 */
void UART_SendHex(uint32_t value);

/*----------------------------------------------------------
 * Receive
 *----------------------------------------------------------*/

/*
 * Check whether a character has been received.
 *
 * Returns:
 *
 *     1 -> data available
 *     0 -> no data
 */
uint8_t UART_DataAvailable(void);

/*
 * Receive one character.
 *
 * Blocking function.
 */
char UART_ReceiveChar(void);

/*
 * Receive one character with timeout.
 */
UART_Status_t UART_ReceiveCharTimeout(
    char *character,
    uint32_t timeout);

/*
 * Receive a line from terminal.
 *
 * Example:
 *
 *     stpo<ENTER>
 *
 * buffer will contain:
 *
 *     "stpo"
 */
UART_Status_t UART_ReceiveLine(
    char *buffer,
    uint32_t buffer_size);

/*----------------------------------------------------------
 * Utility
 *----------------------------------------------------------*/

/*
 * Flush received data.
 */
void UART_FlushReceive(void);

#endif /* UART_H */
