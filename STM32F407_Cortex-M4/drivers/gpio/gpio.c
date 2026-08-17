#include "gpio.h"

/*----------------------------------------------------------
 * Internal Helper
 *----------------------------------------------------------*/

static uint32_t GPIO_GetPortIndex(GPIO_TypeDef *port)
{
    if (port == GPIOA)
    {
        return 0U;
    }

    if (port == GPIOB)
    {
        return 1U;
    }

    if (port == GPIOC)
    {
        return 2U;
    }

    if (port == GPIOD)
    {
        return 3U;
    }

    if (port == GPIOE)
    {
        return 4U;
    }

    if (port == GPIOF)
    {
        return 5U;
    }

    if (port == GPIOG)
    {
        return 6U;
    }

    if (port == GPIOH)
    {
        return 7U;
    }

    return 0xFFFFFFFFU;
}

/*----------------------------------------------------------
 * GPIO_PortInit
 *----------------------------------------------------------*/

GPIO_Status_t GPIO_PortInit(GPIO_TypeDef *port)
{
    uint32_t index;

    if (port == NULL)
    {
        return GPIO_INVALID_PARAMETER;
    }

    index = GPIO_GetPortIndex(port);

    if (index > 7U)
    {
        return GPIO_INVALID_PARAMETER;
    }

    /*
     * Enable GPIO clock.
     *
     * RCC AHB1ENR:
     *
     * Bit 0 -> GPIOA
     * Bit 1 -> GPIOB
     * Bit 2 -> GPIOC
     * Bit 3 -> GPIOD
     * Bit 4 -> GPIOE
     * Bit 5 -> GPIOF
     * Bit 6 -> GPIOG
     * Bit 7 -> GPIOH
     */

    RCC->AHB1ENR |= (1UL << index);

    /*
     * Dummy read to ensure clock activation
     * before accessing GPIO registers.
     */

    (void)RCC->AHB1ENR;

    return GPIO_OK;
}

/*----------------------------------------------------------
 * GPIO_Init
 *----------------------------------------------------------*/

GPIO_Status_t GPIO_Init(GPIO_TypeDef *port,
                        uint8_t pin,
                        const GPIO_Config_t *config)
{
    uint32_t position;
    uint32_t mask;

    if ((port == NULL) ||
        (config == NULL) ||
        (pin > 15U))
    {
        return GPIO_INVALID_PARAMETER;
    }

    if (GPIO_PortInit(port) != GPIO_OK)
    {
        return GPIO_ERROR;
    }

    position = (uint32_t)pin * 2U;

    mask = (3UL << position);

    /*
     * Configure mode.
     */

    port->MODER &= ~mask;

    port->MODER |=
        ((uint32_t)config->mode << position);

    /*
     * Configure output type.
     */

    if (config->output_type == GPIO_OUTPUT_OPEN_DRAIN)
    {
        port->OTYPER |= (1UL << pin);
    }
    else
    {
        port->OTYPER &= ~(1UL << pin);
    }

    /*
     * Configure speed.
     */

    port->OSPEEDR &= ~mask;

    port->OSPEEDR |=
        ((uint32_t)config->speed << position);

    /*
     * Configure pull-up / pull-down.
     */

    port->PUPDR &= ~mask;

    port->PUPDR |=
        ((uint32_t)config->pull << position);

    /*
     * Configure alternate function.
     */

    if (config->mode == GPIO_MODE_AF)
    {
        GPIO_SetAlternateFunction(
            port,
            pin,
            config->alternate_function
        );
    }

    return GPIO_OK;
}

/*----------------------------------------------------------
 * GPIO_ConfigOutput
 *----------------------------------------------------------*/

GPIO_Status_t GPIO_ConfigOutput(GPIO_TypeDef *port,
                                 uint8_t pin)
{
    GPIO_Config_t config;

    config.mode = GPIO_MODE_OUTPUT;

    config.output_type =
        GPIO_OUTPUT_PUSH_PULL;

    config.speed =
        GPIO_SPEED_HIGH;

    config.pull =
        GPIO_PULL_NONE;

    config.alternate_function = 0U;

    return GPIO_Init(
        port,
        pin,
        &config
    );
}

/*----------------------------------------------------------
 * GPIO_Set
 *----------------------------------------------------------*/

void GPIO_Set(GPIO_TypeDef *port,
              uint8_t pin)
{
    if ((port == NULL) || (pin > 15U))
    {
        return;
    }

    /*
     * GPIO BSRR lower 16 bits
     * set the corresponding pin.
     */

    port->BSRR = (1UL << pin);
}

/*----------------------------------------------------------
 * GPIO_Reset
 *----------------------------------------------------------*/

void GPIO_Reset(GPIO_TypeDef *port,
                uint8_t pin)
{
    if ((port == NULL) || (pin > 15U))
    {
        return;
    }

    /*
     * GPIO BSRR upper 16 bits
     * reset the corresponding pin.
     */

    port->BSRR =
        (1UL << (pin + 16U));
}

/*----------------------------------------------------------
 * GPIO_Toggle
 *----------------------------------------------------------*/

void GPIO_Toggle(GPIO_TypeDef *port,
                 uint8_t pin)
{
    if ((port == NULL) || (pin > 15U))
    {
        return;
    }

    if ((port->ODR & (1UL << pin)) != 0U)
    {
        GPIO_Reset(port, pin);
    }
    else
    {
        GPIO_Set(port, pin);
    }
}

/*----------------------------------------------------------
 * GPIO_Write
 *----------------------------------------------------------*/

void GPIO_Write(GPIO_TypeDef *port,
                uint8_t pin,
                GPIO_State_t state)
{
    if (state == GPIO_HIGH)
    {
        GPIO_Set(port, pin);
    }
    else
    {
        GPIO_Reset(port, pin);
    }
}

/*----------------------------------------------------------
 * GPIO_Read
 *----------------------------------------------------------*/

GPIO_State_t GPIO_Read(GPIO_TypeDef *port,
                       uint8_t pin)
{
    if ((port == NULL) || (pin > 15U))
    {
        return GPIO_LOW;
    }

    if ((port->IDR & (1UL << pin)) != 0U)
    {
        return GPIO_HIGH;
    }

    return GPIO_LOW;
}

/*----------------------------------------------------------
 * GPIO_SetAlternateFunction
 *----------------------------------------------------------*/

GPIO_Status_t GPIO_SetAlternateFunction(GPIO_TypeDef *port,
                                        uint8_t pin,
                                        uint8_t alternate_function)
{
    uint32_t index;
    uint32_t shift;

    if ((port == NULL) ||
        (pin > 15U) ||
        (alternate_function > 15U))
    {
        return GPIO_INVALID_PARAMETER;
    }

    /*
     * GPIO alternate-function registers:
     *
     * AFR[0] -> pins 0-7
     * AFR[1] -> pins 8-15
     */

    if (pin < 8U)
    {
        index = 0U;
        shift = (uint32_t)pin * 4U;
    }
    else
    {
        index = 1U;
        shift = ((uint32_t)pin - 8U) * 4U;
    }

    /*
     * Clear existing AF configuration.
     */

    port->AFR[index] &=
        ~(0x0FUL << shift);

    /*
     * Set requested AF.
     */

    port->AFR[index] |=
        ((uint32_t)alternate_function << shift);

    return GPIO_OK;
}
