#ifndef STM32F4XX_HAL_H
#define STM32F4XX_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32f407xx.h"

/*
 * STM32F4xx Lightweight Hardware Abstraction Layer
 *
 * Target:
 *     STM32F407 Cortex-M4
 *
 * This project HAL provides:
 *
 *     - HAL initialization
 *     - System tick
 *     - Millisecond delay
 *     - Microsecond delay
 *     - GPIO abstraction
 *     - Clock information
 *
 * Low-level peripheral drivers such as:
 *
 *     UART
 *     SPI
 *     Flash
 *     LCD
 *     Timer
 *
 * can use this layer where required.
 */

/*----------------------------------------------------------
 * HAL Status
 *----------------------------------------------------------*/

typedef enum
{
    HAL_OK = 0x00U,
    HAL_ERROR = 0x01U,
    HAL_BUSY = 0x02U,
    HAL_TIMEOUT = 0x03U

} HAL_StatusTypeDef;

/*----------------------------------------------------------
 * HAL Lock
 *----------------------------------------------------------*/

typedef enum
{
    HAL_UNLOCKED = 0x00U,
    HAL_LOCKED = 0x01U

} HAL_LockTypeDef;

/*----------------------------------------------------------
 * GPIO Configuration
 *----------------------------------------------------------*/

typedef enum
{
    HAL_GPIO_MODE_INPUT = 0U,
    HAL_GPIO_MODE_OUTPUT,
    HAL_GPIO_MODE_AF,
    HAL_GPIO_MODE_ANALOG

} HAL_GPIO_Mode_t;

/*----------------------------------------------------------
 * GPIO Pull Configuration
 *----------------------------------------------------------*/

typedef enum
{
    HAL_GPIO_NOPULL = 0U,
    HAL_GPIO_PULLUP,
    HAL_GPIO_PULLDOWN

} HAL_GPIO_Pull_t;

/*----------------------------------------------------------
 * GPIO Output Type
 *----------------------------------------------------------*/

typedef enum
{
    HAL_GPIO_PUSH_PULL = 0U,
    HAL_GPIO_OPEN_DRAIN

} HAL_GPIO_OutputType_t;

/*----------------------------------------------------------
 * GPIO Speed
 *----------------------------------------------------------*/

typedef enum
{
    HAL_GPIO_SPEED_LOW = 0U,
    HAL_GPIO_SPEED_MEDIUM,
    HAL_GPIO_SPEED_HIGH,
    HAL_GPIO_SPEED_VERY_HIGH

} HAL_GPIO_Speed_t;

/*----------------------------------------------------------
 * GPIO Pin State
 *----------------------------------------------------------*/

typedef enum
{
    HAL_GPIO_PIN_RESET = 0U,
    HAL_GPIO_PIN_SET = 1U

} HAL_GPIO_PinState;

/*----------------------------------------------------------
 * GPIO Configuration Structure
 *----------------------------------------------------------*/

typedef struct
{
    uint32_t pin;

    HAL_GPIO_Mode_t mode;

    HAL_GPIO_OutputType_t output_type;

    HAL_GPIO_Speed_t speed;

    HAL_GPIO_Pull_t pull;

    uint8_t alternate;

} HAL_GPIO_InitTypeDef;

/*----------------------------------------------------------
 * Clock Information
 *----------------------------------------------------------*/

#define HAL_HCLK_FREQ_DEFAULT       168000000UL
#define HAL_PCLK1_FREQ_DEFAULT       42000000UL
#define HAL_PCLK2_FREQ_DEFAULT       84000000UL

/*----------------------------------------------------------
 * System Tick
 *----------------------------------------------------------*/

#define HAL_TICK_FREQUENCY_HZ       1000UL

/*----------------------------------------------------------
 * HAL Initialization
 *----------------------------------------------------------*/

/*
 * Initialize HAL.
 *
 * Performs:
 *
 *     - FPU configuration
 *     - System clock-related initialization
 *     - SysTick initialization
 */
HAL_StatusTypeDef HAL_Init(void);

/*
 * Deinitialize HAL.
 */
HAL_StatusTypeDef HAL_DeInit(void);

/*----------------------------------------------------------
 * Tick Functions
 *----------------------------------------------------------*/

/*
 * Return current HAL tick in milliseconds.
 */
uint32_t HAL_GetTick(void);

/*
 * Increment system tick.
 *
 * Normally called from SysTick_Handler().
 */
void HAL_IncTick(void);

/*
 * Reset system tick.
 */
void HAL_ResetTick(void);

/*----------------------------------------------------------
 * Delay Functions
 *----------------------------------------------------------*/

/*
 * Delay in milliseconds.
 */
void HAL_Delay(uint32_t milliseconds);

/*
 * Delay in microseconds.
 */
void HAL_DelayUs(uint32_t microseconds);

/*----------------------------------------------------------
 * GPIO Functions
 *----------------------------------------------------------*/

/*
 * Initialize GPIO pin.
 */
HAL_StatusTypeDef HAL_GPIO_Init(
    GPIO_TypeDef *GPIOx,
    const HAL_GPIO_InitTypeDef *config);

/*
 * Deinitialize GPIO pin.
 */
HAL_StatusTypeDef HAL_GPIO_DeInit(
    GPIO_TypeDef *GPIOx,
    uint32_t pin);

/*
 * Write GPIO pin.
 */
void HAL_GPIO_WritePin(
    GPIO_TypeDef *GPIOx,
    uint32_t pin,
    HAL_GPIO_PinState state);

/*
 * Toggle GPIO pin.
 */
void HAL_GPIO_TogglePin(
    GPIO_TypeDef *GPIOx,
    uint32_t pin);

/*
 * Read GPIO pin.
 */
HAL_GPIO_PinState HAL_GPIO_ReadPin(
    GPIO_TypeDef *GPIOx,
    uint32_t pin);

/*----------------------------------------------------------
 * Clock Functions
 *----------------------------------------------------------*/

/*
 * Return system clock frequency.
 */
uint32_t HAL_GetSystemClock(void);

/*
 * Return APB1 clock frequency.
 */
uint32_t HAL_GetPCLK1(void);

/*
 * Return APB2 clock frequency.
 */
uint32_t HAL_GetPCLK2(void);

/*----------------------------------------------------------
 * MSP Initialization
 *----------------------------------------------------------*/

/*
 * Low-level MCU support initialization.
 */
void HAL_MspInit(void);

/*
 * GPIO low-level initialization.
 */
void HAL_GPIO_MspInit(
    GPIO_TypeDef *GPIOx);

/*
 * GPIO low-level deinitialization.
 */
void HAL_GPIO_MspDeInit(
    GPIO_TypeDef *GPIOx);

/*----------------------------------------------------------
 * Assertion
 *----------------------------------------------------------*/

#ifdef USE_FULL_ASSERT

void HAL_AssertFailed(
    const char *file,
    uint32_t line);

#endif

#ifdef __cplusplus
}
#endif

#endif /* STM32F4XX_HAL_H */
