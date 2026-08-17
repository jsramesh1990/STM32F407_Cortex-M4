#ifndef CONFIG_H
#define CONFIG_H

/*
 * ============================================================
 * STM32F407 Cortex-M4
 * Scrambler Word Solver
 *
 * Project Configuration
 * ============================================================
 */

/*----------------------------------------------------------
 * MCU Configuration
 *----------------------------------------------------------*/

#define MCU_NAME                    "STM32F407VG"

#define MCU_CORE                    "Cortex-M4"

#define MCU_CORE_CLOCK_HZ           168000000UL

#define MCU_APB1_CLOCK_HZ            42000000UL

#define MCU_APB2_CLOCK_HZ            84000000UL


/*----------------------------------------------------------
 * UART Configuration
 *----------------------------------------------------------*/

#define CONFIG_UART_BAUD_RATE        115200UL

#define CONFIG_UART_BUFFER_SIZE      128U


/*----------------------------------------------------------
 * SPI Configuration
 *----------------------------------------------------------*/

#define CONFIG_SPI_MODE              0U

#define CONFIG_SPI_CLOCK_DIV         8U


/*----------------------------------------------------------
 * Flash Configuration
 *----------------------------------------------------------*/

#define CONFIG_FLASH_PAGE_SIZE       256U

#define CONFIG_FLASH_SECTOR_SIZE     4096U

#define CONFIG_FLASH_BUFFER_SIZE     256U


/*----------------------------------------------------------
 * Dictionary Configuration
 *----------------------------------------------------------*/

/*
 * Maximum supported word length.
 *
 * Project requirement:
 *
 *     2 - 45 characters
 */

#define CONFIG_MIN_WORD_LENGTH       2U

#define CONFIG_MAX_WORD_LENGTH       45U

#define CONFIG_MAX_DICTIONARY_WORDS  12000U


/*----------------------------------------------------------
 * Scrambler Configuration
 *----------------------------------------------------------*/

#define CONFIG_INPUT_BUFFER_SIZE     46U

#define CONFIG_RESULT_BUFFER_SIZE    64U

#define CONFIG_MAX_RESULTS           64U


/*----------------------------------------------------------
 * LCD Configuration
 *----------------------------------------------------------*/

#define CONFIG_LCD_COLUMNS           16U

#define CONFIG_LCD_ROWS              2U


/*----------------------------------------------------------
 * Timer Configuration
 *----------------------------------------------------------*/

#define CONFIG_TIMER_FREQUENCY_HZ    1000000UL

#define CONFIG_TIMER_TIMEOUT_US      1000000UL


/*----------------------------------------------------------
 * Debug Configuration
 *----------------------------------------------------------*/

#define CONFIG_DEBUG_ENABLE          1U

#define CONFIG_UART_DEBUG_ENABLE     1U


/*----------------------------------------------------------
 * Feature Configuration
 *----------------------------------------------------------*/

#define CONFIG_ENABLE_UART           1U

#define CONFIG_ENABLE_SPI            1U

#define CONFIG_ENABLE_FLASH          1U

#define CONFIG_ENABLE_LCD            1U

#define CONFIG_ENABLE_TIMER          1U


/*----------------------------------------------------------
 * Compiler Configuration
 *----------------------------------------------------------*/

#define CONFIG_USE_FLOAT             0U

#define CONFIG_USE_MALLOC            1U


#endif /* CONFIG_H */
