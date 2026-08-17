#include "board.h"
#include "clock.h"

/*
 * Board initialization
 */
void Board_Init(void)
{
    /*
     * Configure system clock first.
     */
    Clock_Init();

    /*
     * Initialize board GPIO/peripherals.
     *
     * UART, SPI, LCD, etc. will be
     * initialized by their respective
     * drivers.
     */
}

/*
 * Initialize board peripherals
 */
void Board_Peripherals_Init(void)
{
    /*
     * Board-level peripheral configuration.
     *
     * Driver initialization can be
     * called from here later.
     */
}

/*
 * Turn board LED ON
 */
void Board_LED_On(void)
{
    /*
     * STM32F407 GPIO implementation
     * will be added here.
     */
}

/*
 * Turn board LED OFF
 */
void Board_LED_Off(void)
{
    /*
     * GPIO implementation.
     */
}

/*
 * Toggle board LED
 */
void Board_LED_Toggle(void)
{
    /*
     * GPIO toggle implementation.
     */
}

/*
 * Simple delay
 */
void Board_Delay(uint32_t milliseconds)
{
    volatile uint32_t count;

    while (milliseconds--)
    {
        /*
         * Temporary software delay.
         *
         * Later this can use SysTick/Timer.
         */
        for (count = 0U; count < 16000U; count++)
        {
            __asm volatile ("nop");
        }
    }
}
