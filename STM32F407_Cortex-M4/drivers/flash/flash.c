#include "flash.h"
#include "spi.h"
#include "gpio.h"

#include "stm32f407xx.h"

/*----------------------------------------------------------
 * Flash Chip Select
 *
 * Example hardware:
 *
 * SPI1:
 *   PA5 -> SCK
 *   PA6 -> MISO
 *   PA7 -> MOSI
 *
 * Flash CS:
 *   PA4 -> Chip Select
 *----------------------------------------------------------*/

#define FLASH_CS_PORT       GPIOA
#define FLASH_CS_PIN        4U

#define FLASH_TIMEOUT       1000000UL

/*----------------------------------------------------------
 * Chip Select Control
 *----------------------------------------------------------*/

static void Flash_CS_Low(void)
{
    FLASH_CS_PORT->BSRR = (1U << (FLASH_CS_PIN + 16U));
}

static void Flash_CS_High(void)
{
    FLASH_CS_PORT->BSRR = (1U << FLASH_CS_PIN);
}

/*----------------------------------------------------------
 * Flash_Init
 *----------------------------------------------------------*/

FlashStatus_t Flash_Init(void)
{
    /*
     * Configure SPI1.
     *
     * The SPI driver is responsible for configuring:
     *
     * SCK
     * MISO
     * MOSI
     */
    if (SPI_Init() != SPI_OK)
    {
        return FLASH_ERROR;
    }

    /*
     * Configure Flash CS as GPIO output.
     */

    GPIO_ConfigOutput(
        FLASH_CS_PORT,
        FLASH_CS_PIN
    );

    /*
     * Flash must remain deselected when idle.
     */

    Flash_CS_High();

    return FLASH_OK;
}

/*----------------------------------------------------------
 * Flash_ReadID
 *----------------------------------------------------------*/

FlashStatus_t Flash_ReadID(uint8_t *manufacturer,
                           uint8_t *memory_type,
                           uint8_t *capacity)
{
    uint8_t id1;
    uint8_t id2;
    uint8_t id3;

    if ((manufacturer == NULL) ||
        (memory_type == NULL) ||
        (capacity == NULL))
    {
        return FLASH_INVALID_PARAMETER;
    }

    Flash_CS_Low();

    /*
     * JEDEC ID command.
     */

    SPI_Transfer(FLASH_CMD_READ_ID);

    id1 = SPI_Transfer(0xFFU);
    id2 = SPI_Transfer(0xFFU);
    id3 = SPI_Transfer(0xFFU);

    Flash_CS_High();

    *manufacturer = id1;
    *memory_type  = id2;
    *capacity     = id3;

    return FLASH_OK;
}

/*----------------------------------------------------------
 * Flash_ReadStatus
 *----------------------------------------------------------*/

uint8_t Flash_ReadStatus(void)
{
    uint8_t status;

    Flash_CS_Low();

    SPI_Transfer(FLASH_CMD_READ_STATUS);

    status = SPI_Transfer(0xFFU);

    Flash_CS_High();

    return status;
}

/*----------------------------------------------------------
 * Flash_WaitWhileBusy
 *----------------------------------------------------------*/

FlashStatus_t Flash_WaitWhileBusy(uint32_t timeout)
{
    while (timeout > 0U)
    {
        if ((Flash_ReadStatus() & FLASH_STATUS_BUSY) == 0U)
        {
            return FLASH_OK;
        }

        timeout--;
    }

    return FLASH_TIMEOUT;
}

/*----------------------------------------------------------
 * Flash_WriteEnable
 *----------------------------------------------------------*/

FlashStatus_t Flash_WriteEnable(void)
{
    Flash_CS_Low();

    SPI_Transfer(FLASH_CMD_WRITE_ENABLE);

    Flash_CS_High();

    /*
     * Check Write Enable Latch.
     */

    if ((Flash_ReadStatus() & FLASH_STATUS_WEL) == 0U)
    {
        return FLASH_ERROR;
    }

    return FLASH_OK;
}

/*----------------------------------------------------------
 * Flash_WriteDisable
 *----------------------------------------------------------*/

FlashStatus_t Flash_WriteDisable(void)
{
    Flash_CS_Low();

    SPI_Transfer(FLASH_CMD_WRITE_DISABLE);

    Flash_CS_High();

    return FLASH_OK;
}

/*----------------------------------------------------------
 * Flash_Read
 *----------------------------------------------------------*/

FlashStatus_t Flash_Read(uint32_t address,
                         uint8_t *buffer,
                         uint32_t length)
{
    uint32_t i;

    if ((buffer == NULL) || (length == 0U))
    {
        return FLASH_INVALID_PARAMETER;
    }

    Flash_CS_Low();

    /*
     * READ DATA command.
     */

    SPI_Transfer(FLASH_CMD_READ_DATA);

    /*
     * 24-bit Flash address.
     */

    SPI_Transfer((uint8_t)((address >> 16) & 0xFFU));
    SPI_Transfer((uint8_t)((address >> 8) & 0xFFU));
    SPI_Transfer((uint8_t)(address & 0xFFU));

    /*
     * Receive data.
     */

    for (i = 0U; i < length; i++)
    {
        buffer[i] = SPI_Transfer(0xFFU);
    }

    Flash_CS_High();

    return FLASH_OK;
}

/*----------------------------------------------------------
 * Flash_PageProgram
 *----------------------------------------------------------*/

FlashStatus_t Flash_PageProgram(uint32_t address,
                                const uint8_t *data,
                                uint32_t length)
{
    uint32_t i;

    if ((data == NULL) ||
        (length == 0U) ||
        (length > FLASH_PAGE_SIZE))
    {
        return FLASH_INVALID_PARAMETER;
    }

    /*
     * A page program operation must not cross
     * a 256-byte page boundary.
     */

    if (((address & 0xFFU) + length) > FLASH_PAGE_SIZE)
    {
        return FLASH_INVALID_PARAMETER;
    }

    if (Flash_WriteEnable() != FLASH_OK)
    {
        return FLASH_ERROR;
    }

    Flash_CS_Low();

    SPI_Transfer(FLASH_CMD_PAGE_PROGRAM);

    /*
     * Send 24-bit address.
     */

    SPI_Transfer((uint8_t)((address >> 16) & 0xFFU));
    SPI_Transfer((uint8_t)((address >> 8) & 0xFFU));
    SPI_Transfer((uint8_t)(address & 0xFFU));

    /*
     * Send data.
     */

    for (i = 0U; i < length; i++)
    {
        SPI_Transfer(data[i]);
    }

    Flash_CS_High();

    /*
     * Wait until programming finishes.
     */

    return Flash_WaitWhileBusy(FLASH_TIMEOUT);
}

/*----------------------------------------------------------
 * Flash_SectorErase
 *----------------------------------------------------------*/

FlashStatus_t Flash_SectorErase(uint32_t address)
{
    if (Flash_WriteEnable() != FLASH_OK)
    {
        return FLASH_ERROR;
    }

    Flash_CS_Low();

    SPI_Transfer(FLASH_CMD_SECTOR_ERASE);

    SPI_Transfer((uint8_t)((address >> 16) & 0xFFU));
    SPI_Transfer((uint8_t)((address >> 8) & 0xFFU));
    SPI_Transfer((uint8_t)(address & 0xFFU));

    Flash_CS_High();

    return Flash_WaitWhileBusy(FLASH_TIMEOUT);
}

/*----------------------------------------------------------
 * Flash_BlockErase32K
 *----------------------------------------------------------*/

FlashStatus_t Flash_BlockErase32K(uint32_t address)
{
    if (Flash_WriteEnable() != FLASH_OK)
    {
        return FLASH_ERROR;
    }

    Flash_CS_Low();

    SPI_Transfer(FLASH_CMD_BLOCK_ERASE_32K);

    SPI_Transfer((uint8_t)((address >> 16) & 0xFFU));
    SPI_Transfer((uint8_t)((address >> 8) & 0xFFU));
    SPI_Transfer((uint8_t)(address & 0xFFU));

    Flash_CS_High();

    return Flash_WaitWhileBusy(FLASH_TIMEOUT);
}

/*----------------------------------------------------------
 * Flash_BlockErase64K
 *----------------------------------------------------------*/

FlashStatus_t Flash_BlockErase64K(uint32_t address)
{
    if (Flash_WriteEnable() != FLASH_OK)
    {
        return FLASH_ERROR;
    }

    Flash_CS_Low();

    SPI_Transfer(FLASH_CMD_BLOCK_ERASE_64K);

    SPI_Transfer((uint8_t)((address >> 16) & 0xFFU));
    SPI_Transfer((uint8_t)((address >> 8) & 0xFFU));
    SPI_Transfer((uint8_t)(address & 0xFFU));

    Flash_CS_High();

    return Flash_WaitWhileBusy(FLASH_TIMEOUT);
}

/*----------------------------------------------------------
 * Flash_ChipErase
 *----------------------------------------------------------*/

FlashStatus_t Flash_ChipErase(void)
{
    if (Flash_WriteEnable() != FLASH_OK)
    {
        return FLASH_ERROR;
    }

    Flash_CS_Low();

    SPI_Transfer(FLASH_CMD_CHIP_ERASE);

    Flash_CS_High();

    return Flash_WaitWhileBusy(FLASH_TIMEOUT);
}

/*----------------------------------------------------------
 * Flash_ReadDictionary
 *----------------------------------------------------------*/

FlashStatus_t Flash_ReadDictionary(uint32_t offset,
                                    uint8_t *buffer,
                                    uint32_t length)
{
    uint32_t address;

    if (buffer == NULL)
    {
        return FLASH_INVALID_PARAMETER;
    }

    /*
     * Dictionary starts at FLASH_DICTIONARY_ADDRESS.
     */

    address = FLASH_DICTIONARY_ADDRESS + offset;

    return Flash_Read(address, buffer, length);
}
