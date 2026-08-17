#include "spi.h"
#include "gpio.h"

#include "stm32f407xx.h"

/*----------------------------------------------------------
 * SPI1 Pin Configuration
 *
 * PA5 -> SPI1_SCK
 * PA6 -> SPI1_MISO
 * PA7 -> SPI1_MOSI
 *
 * Alternate Function 5
 *----------------------------------------------------------*/

#define SPI1_SCK_PIN           5U
#define SPI1_MISO_PIN          6U
#define SPI1_MOSI_PIN          7U

#define SPI1_GPIO_PORT         GPIOA

#define SPI_TIMEOUT            1000000UL

/*----------------------------------------------------------
 * SPI1 Clock Enable
 *----------------------------------------------------------*/

static void SPI1_EnableClock(void)
{
    /*
     * Enable GPIOA clock.
     */

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /*
     * Enable SPI1 clock.
     *
     * SPI1 is connected to APB2.
     */

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    /*
     * Dummy reads ensure register writes complete.
     */

    (void)RCC->AHB1ENR;
    (void)RCC->APB2ENR;
}

/*----------------------------------------------------------
 * Configure SPI GPIO
 *----------------------------------------------------------*/

static SPI_Status_t SPI1_GPIO_Init(void)
{
    GPIO_Config_t config;

    /*
     * SPI pins use:
     *
     * Alternate Function
     * Push-pull
     * High speed
     * No pull
     * AF5
     */

    config.mode =
        GPIO_MODE_AF;

    config.output_type =
        GPIO_OUTPUT_PUSH_PULL;

    config.speed =
        GPIO_SPEED_VERY_HIGH;

    config.pull =
        GPIO_PULL_NONE;

    config.alternate_function =
        5U;

    /*
     * SCK
     */

    if (GPIO_Init(
            SPI1_GPIO_PORT,
            SPI1_SCK_PIN,
            &config) != GPIO_OK)
    {
        return SPI_ERROR;
    }

    /*
     * MISO
     */

    if (GPIO_Init(
            SPI1_GPIO_PORT,
            SPI1_MISO_PIN,
            &config) != GPIO_OK)
    {
        return SPI_ERROR;
    }

    /*
     * MOSI
     */

    if (GPIO_Init(
            SPI1_GPIO_PORT,
            SPI1_MOSI_PIN,
            &config) != GPIO_OK)
    {
        return SPI_ERROR;
    }

    return SPI_OK;
}

/*----------------------------------------------------------
 * SPI1_InitRegisters
 *----------------------------------------------------------*/

static void SPI1_InitRegisters(const SPI_Config_t *config)
{
    uint32_t cr1;

    /*
     * Disable SPI before configuration.
     */

    SPI1->CR1 &= ~SPI_CR1_SPE;

    /*
     * Start with default configuration.
     */

    cr1 = 0U;

    /*
     * Master mode.
     */

    cr1 |= SPI_CR1_MSTR;

    /*
     * Software slave management.
     *
     * NSS is not controlled by SPI peripheral.
     * External Flash CS is controlled by flash.c.
     */

    cr1 |= SPI_CR1_SSM;
    cr1 |= SPI_CR1_SSI;

    /*
     * SPI mode.
     */

    switch (config->mode)
    {
        case SPI_MODE_0:

            /*
             * CPOL = 0
             * CPHA = 0
             */

            break;

        case SPI_MODE_1:

            /*
             * CPOL = 0
             * CPHA = 1
             */

            cr1 |= SPI_CR1_CPHA;

            break;

        case SPI_MODE_2:

            /*
             * CPOL = 1
             * CPHA = 0
             */

            cr1 |= SPI_CR1_CPOL;

            break;

        case SPI_MODE_3:

            /*
             * CPOL = 1
             * CPHA = 1
             */

            cr1 |= SPI_CR1_CPOL;
            cr1 |= SPI_CR1_CPHA;

            break;

        default:

            break;
    }

    /*
     * Baud rate.
     *
     * BR[2:0]
     */

    cr1 |= ((uint32_t)config->baud_rate << 3U);

    /*
     * 8-bit data frame.
     *
     * DFF = 0
     */

    cr1 &= ~SPI_CR1_DFF;

    /*
     * MSB first.
     */

    cr1 &= ~SPI_CR1_LSBFIRST;

    SPI1->CR1 = cr1;

    /*
     * Clear CR2.
     */

    SPI1->CR2 = 0U;

    /*
     * Enable SPI.
     */

    SPI1->CR1 |= SPI_CR1_SPE;
}

/*----------------------------------------------------------
 * SPI_Init
 *----------------------------------------------------------*/

SPI_Status_t SPI_Init(void)
{
    SPI_Config_t config;

    /*
     * Default configuration for common SPI Flash:
     *
     * Mode 0
     * PCLK / 8
     */

    config.mode =
        SPI_MODE_0;

    config.baud_rate =
        SPI_BAUD_DIV8;

    return SPI_InitConfig(&config);
}

/*----------------------------------------------------------
 * SPI_InitConfig
 *----------------------------------------------------------*/

SPI_Status_t SPI_InitConfig(
    const SPI_Config_t *config)
{
    if (config == NULL)
    {
        return SPI_INVALID_PARAMETER;
    }

    if ((config->mode > SPI_MODE_3) ||
        (config->baud_rate > SPI_BAUD_DIV256))
    {
        return SPI_INVALID_PARAMETER;
    }

    /*
     * Enable peripheral clocks.
     */

    SPI1_EnableClock();

    /*
     * Configure SPI pins.
     */

    if (SPI1_GPIO_Init() != SPI_OK)
    {
        return SPI_ERROR;
    }

    /*
     * Configure SPI peripheral.
     */

    SPI1_InitRegisters(config);

    return SPI_OK;
}

/*----------------------------------------------------------
 * SPI_DeInit
 *----------------------------------------------------------*/

void SPI_DeInit(void)
{
    /*
     * Disable SPI.
     */

    SPI1->CR1 &= ~SPI_CR1_SPE;

    /*
     * Reset SPI1 peripheral.
     */

    RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;

    RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
}

/*----------------------------------------------------------
 * SPI_Transfer
 *----------------------------------------------------------*/

uint8_t SPI_Transfer(uint8_t data)
{
    uint32_t timeout;

    /*
     * Wait until transmit buffer is empty.
     */

    timeout = SPI_TIMEOUT;

    while ((SPI1->SR & SPI_SR_TXE) == 0U)
    {
        if (--timeout == 0U)
        {
            return 0xFFU;
        }
    }

    /*
     * Write data to SPI data register.
     */

    SPI1->DR = data;

    /*
     * Wait until receive buffer contains data.
     */

    timeout = SPI_TIMEOUT;

    while ((SPI1->SR & SPI_SR_RXNE) == 0U)
    {
        if (--timeout == 0U)
        {
            return 0xFFU;
        }
    }

    /*
     * Return received byte.
     */

    return (uint8_t)SPI1->DR;
}

/*----------------------------------------------------------
 * SPI_Transmit
 *----------------------------------------------------------*/

SPI_Status_t SPI_Transmit(
    const uint8_t *data,
    uint32_t length)
{
    uint32_t i;

    if ((data == NULL) ||
        (length == 0U))
    {
        return SPI_INVALID_PARAMETER;
    }

    for (i = 0U; i < length; i++)
    {
        /*
         * SPI_Transfer also receives a byte.
         * The received value is ignored.
         */

        (void)SPI_Transfer(data[i]);
    }

    /*
     * Wait until SPI is no longer busy.
     */

    while ((SPI1->SR & SPI_SR_BSY) != 0U)
    {
        /* Wait */
    }

    return SPI_OK;
}

/*----------------------------------------------------------
 * SPI_Receive
 *----------------------------------------------------------*/

SPI_Status_t SPI_Receive(
    uint8_t *data,
    uint32_t length)
{
    uint32_t i;

    if ((data == NULL) ||
        (length == 0U))
    {
        return SPI_INVALID_PARAMETER;
    }

    for (i = 0U; i < length; i++)
    {
        /*
         * SPI requires clock pulses to receive data.
         *
         * 0xFF is transmitted as dummy data.
         */

        data[i] =
            SPI_Transfer(0xFFU);
    }

    return SPI_OK;
}

/*----------------------------------------------------------
 * SPI_TransmitReceive
 *----------------------------------------------------------*/

SPI_Status_t SPI_TransmitReceive(
    const uint8_t *tx_data,
    uint8_t *rx_data,
    uint32_t length)
{
    uint32_t i;

    if ((tx_data == NULL) ||
        (rx_data == NULL) ||
        (length == 0U))
    {
        return SPI_INVALID_PARAMETER;
    }

    for (i = 0U; i < length; i++)
    {
        rx_data[i] =
            SPI_Transfer(tx_data[i]);
    }

    return SPI_OK;
}
