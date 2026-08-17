#include "stm32f4xx_hal.h"

/*
 * STM32F4xx Lightweight HAL
 *
 * Target:
 *     STM32F407 Cortex-M4
 *
 * Clock configuration assumed:
 *
 *     SYSCLK = 168 MHz
 *     APB1   = 42 MHz
 *     APB2   = 84 MHz
 *
 * SysTick:
 *
 *     1 ms tick
 */

/*----------------------------------------------------------
 * Internal Tick Counter
 *----------------------------------------------------------*/

static volatile uint32_t hal_tick = 0U;

/*----------------------------------------------------------
 * GPIO Clock Enable
 *----------------------------------------------------------*/

static void HAL_GPIO_EnableClock(GPIO_TypeDef *GPIOx)
{
    if (GPIOx == GPIOA)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    }
    else if (GPIOx == GPIOB)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    }
    else if (GPIOx == GPIOC)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    }
    else if (GPIOx == GPIOD)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    }
    else if (GPIOx == GPIOE)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    }
    else if (GPIOx == GPIOF)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    }
    else if (GPIOx == GPIOG)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
    }
    else if (GPIOx == GPIOH)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
    }
    else if (GPIOx == GPIOI)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
    }

    /*
     * Read register to guarantee write completion.
     */

    (void)RCC->AHB1ENR;
}

/*----------------------------------------------------------
 * HAL_Init
 *----------------------------------------------------------*/

HAL_StatusTypeDef HAL_Init(void)
{
    /*
     * Enable FPU access for Cortex-M4.
     *
     * CP10 and CP11:
     *
     *     11 = Full access
     */

    SCB->CPACR |=
        (3UL << 10U) |
        (3UL << 20U);

    /*
     * Enable SysTick.
     *
     * System clock:
     *
     *     168 MHz
     *
     * Tick:
     *
     *     1 ms
     */

    SysTick->LOAD =
        (HAL_HCLK_FREQ_DEFAULT /
         HAL_TICK_FREQUENCY_HZ) - 1UL;

    SysTick->VAL = 0U;

    /*
     * Processor clock.
     */

    SysTick->CTRL =
        SysTick_CTRL_CLKSOURCE_Msk |
        SysTick_CTRL_TICKINT_Msk |
        SysTick_CTRL_ENABLE_Msk;

    /*
     * User low-level initialization.
     */

    HAL_MspInit();

    return HAL_OK;
}

/*----------------------------------------------------------
 * HAL_DeInit
 *----------------------------------------------------------*/

HAL_StatusTypeDef HAL_DeInit(void)
{
    /*
     * Disable SysTick.
     */

    SysTick->CTRL = 0U;

    SysTick->LOAD = 0U;
    SysTick->VAL = 0U;

    /*
     * Reset tick.
     */

    hal_tick = 0U;

    return HAL_OK;
}

/*----------------------------------------------------------
 * HAL_GetTick
 *----------------------------------------------------------*/

uint32_t HAL_GetTick(void)
{
    return hal_tick;
}

/*----------------------------------------------------------
 * HAL_IncTick
 *----------------------------------------------------------*/

void HAL_IncTick(void)
{
    hal_tick++;
}

/*----------------------------------------------------------
 * HAL_ResetTick
 *----------------------------------------------------------*/

void HAL_ResetTick(void)
{
    hal_tick = 0U;
}

/*----------------------------------------------------------
 * HAL_Delay
 *----------------------------------------------------------*/

void HAL_Delay(uint32_t milliseconds)
{
    uint32_t start_tick;

    start_tick = HAL_GetTick();

    /*
     * Unsigned subtraction allows the tick counter
     * to wrap around safely.
     */

    while ((uint32_t)(
        HAL_GetTick() - start_tick) < milliseconds)
    {
        /*
         * Wait for SysTick interrupt.
         */

        __WFI();
    }
}

/*----------------------------------------------------------
 * HAL_DelayUs
 *----------------------------------------------------------*/

void HAL_DelayUs(uint32_t microseconds)
{
    /*
     * Simple cycle-based delay.
     *
     * Approximate only.
     *
     * For accurate microsecond timing use TIM2
     * from the project timer driver.
     */

    uint32_t cycles;

    cycles =
        (microseconds * 168UL) / 4UL;

    while (cycles--)
    {
        __NOP();
    }
}

/*----------------------------------------------------------
 * HAL_GPIO_Init
 *----------------------------------------------------------*/

HAL_StatusTypeDef HAL_GPIO_Init(
    GPIO_TypeDef *GPIOx,
    const HAL_GPIO_InitTypeDef *config)
{
    uint32_t pin;
    uint32_t position;

    if ((GPIOx == NULL) ||
        (config == NULL))
    {
        return HAL_ERROR;
    }

    /*
     * Enable GPIO peripheral clock.
     */

    HAL_GPIO_EnableClock(GPIOx);

    /*
     * Configure every selected pin.
     */

    for (position = 0U;
         position < 16U;
         position++)
    {
        pin = (1UL << position);

        if ((config->pin & pin) == 0U)
        {
            continue;
        }

        /*
         * MODER
         *
         * 00 = Input
         * 01 = Output
         * 10 = Alternate Function
         * 11 = Analog
         */

        GPIOx->MODER &= ~(3UL << (position * 2U));

        switch (config->mode)
        {
            case HAL_GPIO_MODE_INPUT:

                GPIOx->MODER |=
                    (0UL << (position * 2U));

                break;

            case HAL_GPIO_MODE_OUTPUT:

                GPIOx->MODER |=
                    (1UL << (position * 2U));

                break;

            case HAL_GPIO_MODE_AF:

                GPIOx->MODER |=
                    (2UL << (position * 2U));

                break;

            case HAL_GPIO_MODE_ANALOG:

                GPIOx->MODER |=
                    (3UL << (position * 2U));

                break;

            default:

                return HAL_ERROR;
        }

        /*
         * Output type.
         *
         * 0 = Push-pull
         * 1 = Open-drain
         */

        if (config->output_type ==
            HAL_GPIO_OPEN_DRAIN)
        {
            GPIOx->OTYPER |= pin;
        }
        else
        {
            GPIOx->OTYPER &= ~pin;
        }

        /*
         * Output speed.
         */

        GPIOx->OSPEEDR &=
            ~(3UL << (position * 2U));

        GPIOx->OSPEEDR |=
            ((uint32_t)config->speed <<
             (position * 2U));

        /*
         * Pull-up / pull-down.
         *
         * 00 = No pull
         * 01 = Pull-up
         * 10 = Pull-down
         */

        GPIOx->PUPDR &=
            ~(3UL << (position * 2U));

        GPIOx->PUPDR |=
            ((uint32_t)config->pull <<
             (position * 2U));

        /*
         * Alternate function.
         *
         * AFR[0] -> pins 0-7
         * AFR[1] -> pins 8-15
         */

        if (config->mode ==
            HAL_GPIO_MODE_AF)
        {
            uint32_t afr_index;
            uint32_t afr_position;

            afr_index =
                position / 8U;

            afr_position =
                (position % 8U) * 4U;

            GPIOx->AFR[afr_index] &=
                ~(0xFUL << afr_position);

            GPIOx->AFR[afr_index] |=
                ((uint32_t)config->alternate <<
                 afr_position);
        }
    }

    return HAL_OK;
}

/*----------------------------------------------------------
 * HAL_GPIO_DeInit
 *----------------------------------------------------------*/

HAL_StatusTypeDef HAL_GPIO_DeInit(
    GPIO_TypeDef *GPIOx,
    uint32_t pin)
{
    uint32_t position;

    if (GPIOx == NULL)
    {
        return HAL_ERROR;
    }

    for (position = 0U;
         position < 16U;
         position++)
    {
        if ((pin & (1UL << position)) == 0U)
        {
            continue;
        }

        /*
         * Reset pin to input mode.
         */

        GPIOx->MODER &=
            ~(3UL << (position * 2U));

        /*
         * Reset output type.
         */

        GPIOx->OTYPER &=
            ~(1UL << position);

        /*
         * Reset speed.
         */

        GPIOx->OSPEEDR &=
            ~(3UL << (position * 2U));

        /*
         * Remove pull-up/down.
         */

        GPIOx->PUPDR &=
            ~(3UL << (position * 2U));
    }

    return HAL_OK;
}

/*----------------------------------------------------------
 * HAL_GPIO_WritePin
 *----------------------------------------------------------*/

void HAL_GPIO_WritePin(
    GPIO_TypeDef *GPIOx,
    uint32_t pin,
    HAL_GPIO_PinState state)
{
    if (GPIOx == NULL)
    {
        return;
    }

    if (state == HAL_GPIO_PIN_SET)
    {
        /*
         * BSRR lower half sets pin.
         */

        GPIOx->BSRR = pin;
    }
    else
    {
        /*
         * BSRR upper half resets pin.
         */

        GPIOx->BSRR = pin << 16U;
    }
}

/*----------------------------------------------------------
 * HAL_GPIO_TogglePin
 *----------------------------------------------------------*/

void HAL_GPIO_TogglePin(
    GPIO_TypeDef *GPIOx,
    uint32_t pin)
{
    if (GPIOx == NULL)
    {
        return;
    }

    if ((GPIOx->ODR & pin) != 0U)
    {
        GPIOx->BSRR =
            pin << 16U;
    }
    else
    {
        GPIOx->BSRR =
            pin;
    }
}

/*----------------------------------------------------------
 * HAL_GPIO_ReadPin
 *----------------------------------------------------------*/

HAL_GPIO_PinState HAL_GPIO_ReadPin(
    GPIO_TypeDef *GPIOx,
    uint32_t pin)
{
    if (GPIOx == NULL)
    {
        return HAL_GPIO_PIN_RESET;
    }

    if ((GPIOx->IDR & pin) != 0U)
    {
        return HAL_GPIO_PIN_SET;
    }

    return HAL_GPIO_PIN_RESET;
}

/*----------------------------------------------------------
 * HAL_GetSystemClock
 *----------------------------------------------------------*/

uint32_t HAL_GetSystemClock(void)
{
    return HAL_HCLK_FREQ_DEFAULT;
}

/*----------------------------------------------------------
 * HAL_GetPCLK1
 *----------------------------------------------------------*/

uint32_t HAL_GetPCLK1(void)
{
    return HAL_PCLK1_FREQ_DEFAULT;
}

/*----------------------------------------------------------
 * HAL_GetPCLK2
 *----------------------------------------------------------*/

uint32_t HAL_GetPCLK2(void)
{
    return HAL_PCLK2_FREQ_DEFAULT;
}

/*----------------------------------------------------------
 * HAL_MspInit
 *----------------------------------------------------------*/

void HAL_MspInit(void)
{
    /*
     * Global MCU low-level initialization.
     *
     * Add NVIC priority configuration here if required.
     */

    /*
     * Configure priority grouping.
     */

    NVIC_SetPriorityGrouping(3U);
}

/*----------------------------------------------------------
 * HAL_GPIO_MspInit
 *----------------------------------------------------------*/

void HAL_GPIO_MspInit(
    GPIO_TypeDef *GPIOx)
{
    /*
     * GPIO clock initialization is performed
     * inside HAL_GPIO_Init().
     *
     * This function is provided so that the architecture
     * remains compatible with a conventional STM32 HAL.
     */

    (void)GPIOx;
}

/*----------------------------------------------------------
 * HAL_GPIO_MspDeInit
 *----------------------------------------------------------*/

void HAL_GPIO_MspDeInit(
    GPIO_TypeDef *GPIOx)
{
    /*
     * Application-specific GPIO deinitialization
     * can be added here.
     */

    (void)GPIOx;
}

/*----------------------------------------------------------
 * SysTick Handler
 *----------------------------------------------------------*/

void SysTick_Handler(void)
{
    /*
     * Increment HAL millisecond tick.
     */

    HAL_IncTick();
}

/*----------------------------------------------------------
 * Assertion Handler
 *----------------------------------------------------------*/

#ifdef USE_FULL_ASSERT

void HAL_AssertFailed(
    const char *file,
    uint32_t line)
{
    /*
     * Application can override this function.
     */

    (void)file;
    (void)line;

    while (1)
    {
        /*
         * Stay here during assertion failure.
         */
    }
}

#endif
