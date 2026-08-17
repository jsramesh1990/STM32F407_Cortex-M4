#include "uart.h"
#include "gpio.h"

#include "stm32f407xx.h"

/*----------------------------------------------------------
 * USART2 Configuration
 *
 * PA2 -> USART2_TX
 * PA3 -> USART2_RX
 *
 * Alternate Function 7
 *----------------------------------------------------------*/

#define UART_PORT               GPIOA

#define UART_TX_PIN             2U
#define UART_RX_PIN             3U

#define UART_ALTERNATE_FUNCTION 7U

#define UART_DEFAULT_BAUD       115200UL

/*
 * USART2 is connected to APB1.
 *
 * This assumes:
 *
 *     APB1 clock = 42 MHz
 *
 * Typical STM32F407 clock configuration:
 *
 *     SYSCLK = 168 MHz
 *     APB1   = 42 MHz
 *     APB2   = 84 MHz
 */

#define UART_PCLK1_HZ           42000000UL

#define UART_TIMEOUT_COUNT      1000000UL

/*----------------------------------------------------------
 * Internal Delay
 *----------------------------------------------------------*/

static void UART_ShortDelay(void)
{
    volatile uint32_t count;

    for (count = 0U;
         count < 10U;
         count++)
    {
        __NOP();
    }
}

/*----------------------------------------------------------
 * Enable USART2 Clock
 *----------------------------------------------------------*/

static void UART_EnableClock(void)
{
    /*
     * Enable GPIOA clock.
     */

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /*
     * Enable USART2 clock.
     *
     * USART2 is on APB1.
     */

    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /*
     * Ensure clock writes are completed.
     */

    (void)RCC->AHB1ENR;
    (void)RCC->APB1ENR;
}

/*----------------------------------------------------------
 * GPIO Configuration
 *----------------------------------------------------------*/

static UART_Status_t UART_GPIO_Init(void)
{
    GPIO_Config_t config;

    /*
     * USART pins:
     *
     * Alternate Function
     * Push-Pull
     * High Speed
     * Pull-Up
     * AF7
     */

    config.mode =
        GPIO_MODE_AF;

    config.output_type =
        GPIO_OUTPUT_PUSH_PULL;

    config.speed =
        GPIO_SPEED_HIGH;

    config.pull =
        GPIO_PULL_UP;

    config.alternate_function =
        UART_ALTERNATE_FUNCTION;

    /*
     * Configure TX.
     */

    if (GPIO_Init(
            UART_PORT,
            UART_TX_PIN,
            &config) != GPIO_OK)
    {
        return UART_ERROR;
    }

    /*
     * Configure RX.
     */

    if (GPIO_Init(
            UART_PORT,
            UART_RX_PIN,
            &config) != GPIO_OK)
    {
        return UART_ERROR;
    }

    return UART_OK;
}

/*----------------------------------------------------------
 * Calculate USART BRR
 *----------------------------------------------------------*/

static uint32_t UART_CalculateBRR(
    uint32_t pclk,
    uint32_t baud)
{
    /*
     * USART oversampling by 16.
     *
     * BRR ≈ PCLK / baud
     */

    return (pclk + (baud / 2U)) / baud;
}

/*----------------------------------------------------------
 * UART_Init
 *----------------------------------------------------------*/

UART_Status_t UART_Init(void)
{
    UART_Config_t config;

    config.baud_rate =
        UART_DEFAULT_BAUD;

    return UART_InitConfig(&config);
}

/*----------------------------------------------------------
 * UART_InitConfig
 *----------------------------------------------------------*/

UART_Status_t UART_InitConfig(
    const UART_Config_t *config)
{
    uint32_t brr;

    if (config == NULL)
    {
        return UART_INVALID_PARAMETER;
    }

    if (config->baud_rate == 0U)
    {
        return UART_INVALID_PARAMETER;
    }

    /*
     * Enable clocks.
     */

    UART_EnableClock();

    /*
     * Configure GPIO.
     */

    if (UART_GPIO_Init() != UART_OK)
    {
        return UART_ERROR;
    }

    /*
     * Disable USART during configuration.
     */

    USART2->CR1 = 0U;
    USART2->CR2 = 0U;
    USART2->CR3 = 0U;

    /*
     * Configure baud rate.
     */

    brr =
        UART_CalculateBRR(
            UART_PCLK1_HZ,
            config->baud_rate
        );

    USART2->BRR = brr;

    /*
     * USART configuration:
     *
     * 8-bit word length
     * No parity
     * 1 stop bit
     *
     * TX enabled
     * RX enabled
     */

    USART2->CR1 =
        USART_CR1_TE |
        USART_CR1_RE;

    /*
     * Enable USART.
     */

    USART2->CR1 |= USART_CR1_UE;

    /*
     * Small stabilization delay.
     */

    UART_ShortDelay();

    return UART_OK;
}

/*----------------------------------------------------------
 * UART_DeInit
 *----------------------------------------------------------*/

void UART_DeInit(void)
{
    /*
     * Disable USART.
     */

    USART2->CR1 &= ~USART_CR1_UE;

    /*
     * Reset USART2 peripheral.
     */

    RCC->APB1RSTR |= RCC_APB1RSTR_USART2RST;

    RCC->APB1RSTR &= ~RCC_APB1RSTR_USART2RST;
}

/*----------------------------------------------------------
 * UART_SendChar
 *----------------------------------------------------------*/

void UART_SendChar(char character)
{
    /*
     * Wait until transmit data register is empty.
     */

    while ((USART2->SR & USART_SR_TXE) == 0U)
    {
        /* Wait */
    }

    /*
     * Send character.
     */

    USART2->DR =
        (uint8_t)character;

    /*
     * Wait until transmission is complete.
     *
     * This is useful before changing direction
     * or shutting down the peripheral.
     */

    while ((USART2->SR & USART_SR_TC) == 0U)
    {
        /* Wait */
    }
}

/*----------------------------------------------------------
 * UART_SendString
 *----------------------------------------------------------*/

void UART_SendString(const char *string)
{
    if (string == NULL)
    {
        return;
    }

    while (*string != '\0')
    {
        UART_SendChar(*string);

        string++;
    }
}

/*----------------------------------------------------------
 * UART_SendLine
 *----------------------------------------------------------*/

void UART_SendLine(const char *string)
{
    UART_SendString(string);

    UART_SendString("\r\n");
}

/*----------------------------------------------------------
 * UART_SendUInt
 *----------------------------------------------------------*/

void UART_SendUInt(uint32_t value)
{
    char buffer[10];

    uint32_t index = 0U;

    /*
     * Special case for zero.
     */

    if (value == 0U)
    {
        UART_SendChar('0');
        return;
    }

    /*
     * Convert number to ASCII.
     */

    while (value > 0U)
    {
        buffer[index++] =
            (char)('0' + (value % 10U));

        value /= 10U;
    }

    /*
     * Print in reverse order.
     */

    while (index > 0U)
    {
        UART_SendChar(
            buffer[--index]
        );
    }
}

/*----------------------------------------------------------
 * UART_SendInt
 *----------------------------------------------------------*/

void UART_SendInt(int32_t value)
{
    uint32_t number;

    if (value < 0)
    {
        UART_SendChar('-');

        /*
         * Safe conversion of negative value.
         */

        number =
            (uint32_t)(-(value + 1));

        number += 1U;
    }
    else
    {
        number =
            (uint32_t)value;
    }

    UART_SendUInt(number);
}

/*----------------------------------------------------------
 * UART_SendHex
 *----------------------------------------------------------*/

void UART_SendHex(uint32_t value)
{
    static const char hex[] =
        "0123456789ABCDEF";

    int32_t shift;

    UART_SendString("0x");

    /*
     * Print 8 hexadecimal digits.
     */

    for (shift = 28;
         shift >= 0;
         shift -= 4)
    {
        UART_SendChar(
            hex[(value >> shift) & 0x0FU]
        );
    }
}

/*----------------------------------------------------------
 * UART_DataAvailable
 *----------------------------------------------------------*/

uint8_t UART_DataAvailable(void)
{
    if ((USART2->SR & USART_SR_RXNE) != 0U)
    {
        return 1U;
    }

    return 0U;
}

/*----------------------------------------------------------
 * UART_ReceiveChar
 *----------------------------------------------------------*/

char UART_ReceiveChar(void)
{
    /*
     * Wait until RX data is available.
     */

    while ((USART2->SR & USART_SR_RXNE) == 0U)
    {
        /* Wait */
    }

    /*
     * Return received character.
     */

    return (char)(USART2->DR & 0xFFU);
}

/*----------------------------------------------------------
 * UART_ReceiveCharTimeout
 *----------------------------------------------------------*/

UART_Status_t UART_ReceiveCharTimeout(
    char *character,
    uint32_t timeout)
{
    if (character == NULL)
    {
        return UART_INVALID_PARAMETER;
    }

    while (timeout > 0U)
    {
        if ((USART2->SR & USART_SR_RXNE) != 0U)
        {
            *character =
                (char)(USART2->DR & 0xFFU);

            return UART_OK;
        }

        timeout--;
    }

    return UART_TIMEOUT;
}

/*----------------------------------------------------------
 * UART_ReceiveLine
 *----------------------------------------------------------*/

UART_Status_t UART_ReceiveLine(
    char *buffer,
    uint32_t buffer_size)
{
    uint32_t index = 0U;

    char character;

    if ((buffer == NULL) ||
        (buffer_size < 2U))
    {
        return UART_INVALID_PARAMETER;
    }

    while (1)
    {
        character =
            UART_ReceiveChar();

        /*
         * ENTER key.
         *
         * Handle both:
         *
         *     \r
         *     \n
         */

        if ((character == '\r') ||
            (character == '\n'))
        {
            /*
             * Null terminate string.
             */

            buffer[index] = '\0';

            UART_SendString("\r\n");

            return UART_OK;
        }

        /*
         * Backspace handling.
         */

        if ((character == '\b') ||
            (character == 127))
        {
            if (index > 0U)
            {
                index--;

                UART_SendString(
                    "\b \b"
                );
            }

            continue;
        }

        /*
         * Store character if there is space.
         *
         * One byte must remain for '\0'.
         */

        if (index < (buffer_size - 1U))
        {
            buffer[index++] =
                character;

            /*
             * Echo character.
             */

            UART_SendChar(character);
        }
    }
}

/*----------------------------------------------------------
 * UART_FlushReceive
 *----------------------------------------------------------*/

void UART_FlushReceive(void)
{
    /*
     * Read and discard all pending data.
     */

    while ((USART2->SR & USART_SR_RXNE) != 0U)
    {
        (void)USART2->DR;
    }
}
