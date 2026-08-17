#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>

/*
 * ============================================================
 * STM32F407 Scrambler Word Solver
 *
 * Project Error Definitions
 * ============================================================
 */

/*----------------------------------------------------------
 * Error Codes
 *----------------------------------------------------------*/

typedef enum
{
    ERROR_NONE = 0,

    /* General errors */

    ERROR_UNKNOWN,

    ERROR_INVALID_PARAMETER,

    ERROR_NOT_INITIALIZED,

    ERROR_ALREADY_INITIALIZED,

    ERROR_TIMEOUT,

    ERROR_BUSY,

    /* Memory errors */

    ERROR_MEMORY,

    ERROR_BUFFER_FULL,

    ERROR_BUFFER_EMPTY,

    /* UART errors */

    ERROR_UART_INIT,

    ERROR_UART_TIMEOUT,

    ERROR_UART_RX,

    ERROR_UART_TX,

    /* SPI errors */

    ERROR_SPI_INIT,

    ERROR_SPI_TIMEOUT,

    ERROR_SPI_TRANSFER,

    /* Flash errors */

    ERROR_FLASH_INIT,

    ERROR_FLASH_READ,

    ERROR_FLASH_WRITE,

    ERROR_FLASH_ERASE,

    ERROR_FLASH_ID,

    /* Dictionary errors */

    ERROR_DICTIONARY_INIT,

    ERROR_DICTIONARY_LOAD,

    ERROR_DICTIONARY_EMPTY,

    ERROR_DICTIONARY_INVALID,

    ERROR_WORD_INVALID,

    ERROR_WORD_TOO_SHORT,

    ERROR_WORD_TOO_LONG,

    /* Scrambler errors */

    ERROR_SCRAMBLER_INIT,

    ERROR_SCRAMBLER_NO_MATCH,

    ERROR_RESULT_FULL,

    /* LCD errors */

    ERROR_LCD_INIT,

    ERROR_LCD_WRITE,

    /* Timer errors */

    ERROR_TIMER_INIT,

    ERROR_TIMER_TIMEOUT

} error_code_t;


/*----------------------------------------------------------
 * Error Context
 *----------------------------------------------------------*/

typedef struct
{
    error_code_t code;

    const char *file;

    uint32_t line;

} error_info_t;


/*----------------------------------------------------------
 * Global Error State
 *----------------------------------------------------------*/

void Error_Set(
    error_code_t code,
    const char *file,
    uint32_t line);

void Error_Clear(void);

error_code_t Error_Get(void);

const char *Error_GetString(
    error_code_t code);

const error_info_t *Error_GetInfo(void);


/*----------------------------------------------------------
 * Convenience Macros
 *----------------------------------------------------------*/

#define ERROR_SET(code) \
    Error_Set((code), __FILE__, __LINE__)


#define ERROR_RETURN(code) \
    do \
    { \
        Error_Set((code), __FILE__, __LINE__); \
        return (code); \
    } while (0)


#endif /* ERROR_H */
