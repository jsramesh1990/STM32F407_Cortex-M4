#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "stm32f407xx.h"

/*
 * STM32F407 SPI Driver
 *
 * SPI1:
 *
 * PA5 -> SCK
 * PA6 -> MISO
 * PA7 -> MOSI
 *
 * Chip Select is handled by the device driver
 * (for example flash.c), not by this SPI driver.
 */

/*----------------------------------------------------------
 * SPI Status
 *----------------------------------------------------------*/

typedef enum
{
    SPI_OK = 0,
    SPI_ERROR,
    SPI_TIMEOUT,
    SPI_INVALID_PARAMETER

} SPI_Status_t;

/*----------------------------------------------------------
 * SPI Configuration
 *----------------------------------------------------------*/

typedef enum
{
    SPI_MODE_0 = 0,     /* CPOL = 0, CPHA = 0 */
    SPI_MODE_1,         /* CPOL = 0, CPHA = 1 */
    SPI_MODE_2,         /* CPOL = 1, CPHA = 0 */
    SPI_MODE_3          /* CPOL = 1, CPHA = 1 */

} SPI_Mode_t;

typedef enum
{
    SPI_BAUD_DIV2 = 0,
    SPI_BAUD_DIV4,
    SPI_BAUD_DIV8,
    SPI_BAUD_DIV16,
    SPI_BAUD_DIV32,
    SPI_BAUD_DIV64,
    SPI_BAUD_DIV128,
    SPI_BAUD_DIV256

} SPI_BaudRate_t;

/*----------------------------------------------------------
 * SPI Configuration Structure
 *----------------------------------------------------------*/

typedef struct
{
    SPI_Mode_t mode;

    SPI_BaudRate_t baud_rate;

} SPI_Config_t;

/*----------------------------------------------------------
 * SPI Initialization
 *----------------------------------------------------------*/

/*
 * Initialize SPI1 using default configuration.
 *
 * Default:
 *   SPI Mode 0
 *   Clock = PCLK / 8
 */
SPI_Status_t SPI_Init(void);

/*
 * Initialize SPI1 using supplied configuration.
 */
SPI_Status_t SPI_InitConfig(const SPI_Config_t *config);

/*
 * Deinitialize SPI1.
 */
void SPI_DeInit(void);

/*----------------------------------------------------------
 * SPI Data Transfer
 *----------------------------------------------------------*/

/*
 * Transfer one byte.
 *
 * SPI is full duplex:
 *
 * TX -> MOSI
 * RX <- MISO
 */
uint8_t SPI_Transfer(uint8_t data);

/*
 * Transmit multiple bytes.
 */
SPI_Status_t SPI_Transmit(const uint8_t *data,
                          uint32_t length);

/*
 * Receive multiple bytes.
 */
SPI_Status_t SPI_Receive(uint8_t *data,
                         uint32_t length);

/*
 * Transmit and receive simultaneously.
 */
SPI_Status_t SPI_TransmitReceive(const uint8_t *tx_data,
                                 uint8_t *rx_data,
                                 uint32_t length);

#endif /* SPI_H */
