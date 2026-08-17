#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "stm32f407xx.h"

/*
 * GPIO Driver
 *
 * STM32F407 Cortex-M4
 *
 * Supports:
 *   - GPIO input
 *   - GPIO output
 *   - GPIO alternate function
 *   - GPIO set/reset
 *   - GPIO read
 *   - GPIO toggle
 */

/*----------------------------------------------------------
 * GPIO Return Status
 *----------------------------------------------------------*/

typedef enum
{
    GPIO_OK = 0,
    GPIO_ERROR,
    GPIO_INVALID_PARAMETER

} GPIO_Status_t;

/*----------------------------------------------------------
 * GPIO Mode
 *----------------------------------------------------------*/

typedef enum
{
    GPIO_MODE_INPUT  = 0x00U,
    GPIO_MODE_OUTPUT = 0x01U,
    GPIO_MODE_AF     = 0x02U,
    GPIO_MODE_ANALOG = 0x03U

} GPIO_Mode_t;

/*----------------------------------------------------------
 * GPIO Output Type
 *----------------------------------------------------------*/

typedef enum
{
    GPIO_OUTPUT_PUSH_PULL  = 0x00U,
    GPIO_OUTPUT_OPEN_DRAIN = 0x01U

} GPIO_OutputType_t;

/*----------------------------------------------------------
 * GPIO Speed
 *----------------------------------------------------------*/

typedef enum
{
    GPIO_SPEED_LOW       = 0x00U,
    GPIO_SPEED_MEDIUM    = 0x01U,
    GPIO_SPEED_HIGH      = 0x02U,
    GPIO_SPEED_VERY_HIGH = 0x03U

} GPIO_Speed_t;

/*----------------------------------------------------------
 * GPIO Pull Configuration
 *----------------------------------------------------------*/

typedef enum
{
    GPIO_PULL_NONE = 0x00U,
    GPIO_PULL_UP   = 0x01U,
    GPIO_PULL_DOWN = 0x02U

} GPIO_Pull_t;

/*----------------------------------------------------------
 * GPIO Pin State
 *----------------------------------------------------------*/

typedef enum
{
    GPIO_LOW  = 0U,
    GPIO_HIGH = 1U

} GPIO_State_t;

/*----------------------------------------------------------
 * GPIO Configuration Structure
 *----------------------------------------------------------*/

typedef struct
{
    GPIO_Mode_t mode;

    GPIO_OutputType_t output_type;

    GPIO_Speed_t speed;

    GPIO_Pull_t pull;

    uint8_t alternate_function;

} GPIO_Config_t;

/*----------------------------------------------------------
 * Initialization
 *----------------------------------------------------------*/

/*
 * Enable GPIO peripheral clock.
 */
GPIO_Status_t GPIO_PortInit(GPIO_TypeDef *port);

/*
 * Configure GPIO pin using configuration structure.
 */
GPIO_Status_t GPIO_Init(GPIO_TypeDef *port,
                        uint8_t pin,
                        const GPIO_Config_t *config);

/*
 * Simple output configuration.
 *
 * Used by flash.c.
 */
GPIO_Status_t GPIO_ConfigOutput(GPIO_TypeDef *port,
                                 uint8_t pin);

/*----------------------------------------------------------
 * GPIO Output
 *----------------------------------------------------------*/

/*
 * Set GPIO pin HIGH.
 */
void GPIO_Set(GPIO_TypeDef *port,
              uint8_t pin);

/*
 * Set GPIO pin LOW.
 */
void GPIO_Reset(GPIO_TypeDef *port,
                uint8_t pin);

/*
 * Toggle GPIO pin.
 */
void GPIO_Toggle(GPIO_TypeDef *port,
                 uint8_t pin);

/*
 * Write GPIO pin state.
 */
void GPIO_Write(GPIO_TypeDef *port,
                uint8_t pin,
                GPIO_State_t state);

/*----------------------------------------------------------
 * GPIO Input
 *----------------------------------------------------------*/

/*
 * Read GPIO pin.
 */
GPIO_State_t GPIO_Read(GPIO_TypeDef *port,
                       uint8_t pin);

/*----------------------------------------------------------
 * Alternate Function
 *----------------------------------------------------------*/

/*
 * Configure alternate-function number.
 */
GPIO_Status_t GPIO_SetAlternateFunction(GPIO_TypeDef *port,
                                        uint8_t pin,
                                        uint8_t alternate_function);

#endif /* GPIO_H */
