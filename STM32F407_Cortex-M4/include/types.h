#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>

/*
 * ============================================================
 * Common Project Types
 * ============================================================
 */

/*----------------------------------------------------------
 * Boolean Type
 *----------------------------------------------------------*/

typedef enum
{
    FALSE = 0,
    TRUE  = 1

} bool_t;


/*----------------------------------------------------------
 * Generic Status
 *----------------------------------------------------------*/

typedef enum
{
    STATUS_OK = 0,
    STATUS_ERROR,
    STATUS_TIMEOUT,
    STATUS_BUSY,
    STATUS_INVALID_PARAMETER,
    STATUS_NOT_FOUND,
    STATUS_FULL,
    STATUS_EMPTY

} status_t;


/*----------------------------------------------------------
 * Enable / Disable
 *----------------------------------------------------------*/

typedef enum
{
    DISABLE = 0,
    ENABLE  = 1

} enable_t;


/*----------------------------------------------------------
 * On / Off
 *----------------------------------------------------------*/

typedef enum
{
    OFF = 0,
    ON  = 1

} state_t;


/*----------------------------------------------------------
 * GPIO State
 *----------------------------------------------------------*/

typedef enum
{
    GPIO_LOW = 0,
    GPIO_HIGH = 1

} gpio_state_t;


/*----------------------------------------------------------
 * Word Structure
 *----------------------------------------------------------*/

typedef struct
{
    char *data;

    uint32_t length;

} word_t;


/*----------------------------------------------------------
 * Dictionary Entry
 *----------------------------------------------------------*/

typedef struct
{
    char *word;

    uint32_t length;

} dictionary_entry_t;


/*----------------------------------------------------------
 * Scrambler Result
 *----------------------------------------------------------*/

typedef struct
{
    char *word;

    uint32_t length;

} scrambler_result_t;


/*----------------------------------------------------------
 * Search Statistics
 *----------------------------------------------------------*/

typedef struct
{
    uint32_t words_checked;

    uint32_t matches_found;

    uint32_t execution_time_us;

} search_statistics_t;


/*----------------------------------------------------------
 * Scrambler Context
 *----------------------------------------------------------*/

typedef struct
{
    char input[46];

    uint32_t input_length;

    uint32_t result_count;

    search_statistics_t statistics;

} scrambler_context_t;


/*----------------------------------------------------------
 * Buffer Structure
 *----------------------------------------------------------*/

typedef struct
{
    uint8_t *data;

    uint32_t size;

    uint32_t length;

    uint32_t position;

} buffer_t;


/*----------------------------------------------------------
 * Version Information
 *----------------------------------------------------------*/

typedef struct
{
    uint16_t major;

    uint16_t minor;

    uint16_t patch;

} version_t;


#endif /* TYPES_H */
