#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>
#include <stddef.h>

/*
 * External SPI Flash Driver
 *
 * STM32F407 Cortex-M4
 *
 * Flash interface:
 *      STM32F407
 *          |
 *          | SPI1
 *          |
 *      External SPI Flash
 *
 * The dictionary file words.bin can be stored
 * in the external Flash memory.
 */

/*----------------------------------------------------------
 * Flash Commands
 *----------------------------------------------------------*/

#define FLASH_CMD_WRITE_ENABLE       0x06U
#define FLASH_CMD_WRITE_DISABLE      0x04U

#define FLASH_CMD_READ_DATA          0x03U
#define FLASH_CMD_FAST_READ          0x0BU

#define FLASH_CMD_PAGE_PROGRAM       0x02U

#define FLASH_CMD_SECTOR_ERASE       0x20U
#define FLASH_CMD_BLOCK_ERASE_32K    0x52U
#define FLASH_CMD_BLOCK_ERASE_64K    0xD8U
#define FLASH_CMD_CHIP_ERASE         0xC7U

#define FLASH_CMD_READ_STATUS        0x05U
#define FLASH_CMD_READ_ID            0x9FU

/*----------------------------------------------------------
 * Flash Status Register
 *----------------------------------------------------------*/

#define FLASH_STATUS_BUSY            (1U << 0)
#define FLASH_STATUS_WEL             (1U << 1)

/*----------------------------------------------------------
 * Flash Geometry
 *----------------------------------------------------------*/

#define FLASH_PAGE_SIZE              256U
#define FLASH_SECTOR_SIZE            4096U
#define FLASH_BLOCK_SIZE_32K         32768U
#define FLASH_BLOCK_SIZE_64K         65536U

/*
 * Dictionary storage location.
 *
 * words.bin will start from this address.
 */
#define FLASH_DICTIONARY_ADDRESS     0x00000000UL

/*----------------------------------------------------------
 * Return Status
 *----------------------------------------------------------*/

typedef enum
{
    FLASH_OK = 0,
    FLASH_ERROR,
    FLASH_TIMEOUT,
    FLASH_INVALID_PARAMETER

} FlashStatus_t;

/*----------------------------------------------------------
 * API
 *----------------------------------------------------------*/

/*
 * Initialize external Flash.
 */
FlashStatus_t Flash_Init(void);

/*
 * Read Flash JEDEC ID.
 *
 * manufacturer : Manufacturer ID
 * memory_type  : Memory type
 * capacity     : Memory capacity code
 */
FlashStatus_t Flash_ReadID(uint8_t *manufacturer,
                           uint8_t *memory_type,
                           uint8_t *capacity);

/*
 * Read Flash status register.
 */
uint8_t Flash_ReadStatus(void);

/*
 * Wait until Flash is no longer busy.
 */
FlashStatus_t Flash_WaitWhileBusy(uint32_t timeout);

/*
 * Read arbitrary data from Flash.
 */
FlashStatus_t Flash_Read(uint32_t address,
                         uint8_t *buffer,
                         uint32_t length);

/*
 * Enable Flash write operation.
 */
FlashStatus_t Flash_WriteEnable(void);

/*
 * Disable Flash write operation.
 */
FlashStatus_t Flash_WriteDisable(void);

/*
 * Write one page.
 *
 * length must not exceed FLASH_PAGE_SIZE.
 */
FlashStatus_t Flash_PageProgram(uint32_t address,
                                const uint8_t *data,
                                uint32_t length);

/*
 * Erase one 4-KB sector.
 */
FlashStatus_t Flash_SectorErase(uint32_t address);

/*
 * Erase 32-KB block.
 */
FlashStatus_t Flash_BlockErase32K(uint32_t address);

/*
 * Erase 64-KB block.
 */
FlashStatus_t Flash_BlockErase64K(uint32_t address);

/*
 * Erase entire Flash.
 */
FlashStatus_t Flash_ChipErase(void);

/*
 * Read dictionary data.
 *
 * This is a convenience API used by dictionary.c.
 */
FlashStatus_t Flash_ReadDictionary(uint32_t offset,
                                    uint8_t *buffer,
                                    uint32_t length);

#endif /* FLASH_H */
