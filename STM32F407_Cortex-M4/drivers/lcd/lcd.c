#include "lcd.h"
#include "gpio.h"

#include "stm32f407xx.h"

/*
 * Delay function.
 *
 * For a production project this should preferably use
 * the STM32 timer/SysTick driver rather than a software loop.
 */
static void LCD_Delay(volatile uint32_t count)
{
    while (count--)
    {
        __NOP();
    }
}

/*----------------------------------------------------------
 * LCD GPIO Configuration
 *
 * Example configuration:
 *
 * LCD RS -> PB0
 * LCD EN -> PB1
 *
 * LCD D4 -> PB2
 * LCD D5 -> PB3
 * LCD D6 -> PB4
 * LCD D7 -> PB5
 *
 * LCD RW -> GND
 *----------------------------------------------------------*/

#define LCD_PORT            GPIOB

#define LCD_RS_PIN          0U
#define LCD_EN_PIN          1U

#define LCD_D4_PIN          2U
#define LCD_D5_PIN          3U
#define LCD_D6_PIN          4U
#define LCD_D7_PIN          5U

/*----------------------------------------------------------
 * LCD GPIO Helpers
 *----------------------------------------------------------*/

static void LCD_RS_High(void)
{
    GPIO_Set(LCD_PORT, LCD_RS_PIN);
}

static void LCD_RS_Low(void)
{
    GPIO_Reset(LCD_PORT, LCD_RS_PIN);
}

static void LCD_EnableHigh(void)
{
    GPIO_Set(LCD_PORT, LCD_EN_PIN);
}

static void LCD_EnableLow(void)
{
    GPIO_Reset(LCD_PORT, LCD_EN_PIN);
}

/*----------------------------------------------------------
 * Write 4-bit Data
 *----------------------------------------------------------*/

static void LCD_WriteNibble(uint8_t nibble)
{
    /*
     * D4
     */
    if (nibble & 0x01U)
    {
        GPIO_Set(LCD_PORT, LCD_D4_PIN);
    }
    else
    {
        GPIO_Reset(LCD_PORT, LCD_D4_PIN);
    }

    /*
     * D5
     */
    if (nibble & 0x02U)
    {
        GPIO_Set(LCD_PORT, LCD_D5_PIN);
    }
    else
    {
        GPIO_Reset(LCD_PORT, LCD_D5_PIN);
    }

    /*
     * D6
     */
    if (nibble & 0x04U)
    {
        GPIO_Set(LCD_PORT, LCD_D6_PIN);
    }
    else
    {
        GPIO_Reset(LCD_PORT, LCD_D6_PIN);
    }

    /*
     * D7
     */
    if (nibble & 0x08U)
    {
        GPIO_Set(LCD_PORT, LCD_D7_PIN);
    }
    else
    {
        GPIO_Reset(LCD_PORT, LCD_D7_PIN);
    }

    /*
     * Enable pulse.
     */

    LCD_EnableHigh();

    LCD_Delay(1000U);

    LCD_EnableLow();

    LCD_Delay(1000U);
}

/*----------------------------------------------------------
 * Write Byte
 *----------------------------------------------------------*/

static void LCD_WriteByte(uint8_t value)
{
    /*
     * Send upper nibble first.
     */

    LCD_WriteNibble(
        (uint8_t)((value >> 4U) & 0x0FU)
    );

    /*
     * Send lower nibble.
     */

    LCD_WriteNibble(
        (uint8_t)(value & 0x0FU)
    );
}

/*----------------------------------------------------------
 * LCD_Init
 *----------------------------------------------------------*/

LCD_Status_t LCD_Init(void)
{
    /*
     * Configure LCD GPIO pins.
     */

    if (GPIO_ConfigOutput(LCD_PORT, LCD_RS_PIN)
        != GPIO_OK)
    {
        return LCD_ERROR;
    }

    if (GPIO_ConfigOutput(LCD_PORT, LCD_EN_PIN)
        != GPIO_OK)
    {
        return LCD_ERROR;
    }

    if (GPIO_ConfigOutput(LCD_PORT, LCD_D4_PIN)
        != GPIO_OK)
    {
        return LCD_ERROR;
    }

    if (GPIO_ConfigOutput(LCD_PORT, LCD_D5_PIN)
        != GPIO_OK)
    {
        return LCD_ERROR;
    }

    if (GPIO_ConfigOutput(LCD_PORT, LCD_D6_PIN)
        != GPIO_OK)
    {
        return LCD_ERROR;
    }

    if (GPIO_ConfigOutput(LCD_PORT, LCD_D7_PIN)
        != GPIO_OK)
    {
        return LCD_ERROR;
    }

    /*
     * Initial states.
     */

    LCD_RS_Low();
    LCD_EnableLow();

    GPIO_Reset(LCD_PORT, LCD_D4_PIN);
    GPIO_Reset(LCD_PORT, LCD_D5_PIN);
    GPIO_Reset(LCD_PORT, LCD_D6_PIN);
    GPIO_Reset(LCD_PORT, LCD_D7_PIN);

    /*
     * LCD power-up delay.
     */

    LCD_Delay(100000U);

    /*
     * HD44780 initialization sequence.
     *
     * Start in 8-bit initialization mode,
     * then switch to 4-bit mode.
     */

    LCD_WriteNibble(0x03U);
    LCD_Delay(20000U);

    LCD_WriteNibble(0x03U);
    LCD_Delay(5000U);

    LCD_WriteNibble(0x03U);
    LCD_Delay(5000U);

    /*
     * Select 4-bit interface.
     */

    LCD_WriteNibble(0x02U);

    LCD_Delay(5000U);

    /*
     * Function set:
     *
     * 4-bit
     * 2-line
     * 5x8 font
     */

    LCD_SendCommand(
        LCD_CMD_FUNCTION_SET |
        LCD_CMD_4_BIT_MODE |
        LCD_CMD_2_LINE |
        LCD_CMD_5X8_DOTS
    );

    /*
     * Display ON
     * Cursor OFF
     * Blink OFF
     */

    LCD_SendCommand(
        LCD_CMD_DISPLAY_CTRL |
        LCD_DISPLAY_ON |
        LCD_CURSOR_OFF |
        LCD_BLINK_OFF
    );

    /*
     * Clear display.
     */

    LCD_Clear();

    /*
     * Entry mode:
     *
     * Increment cursor
     * No display shift
     */

    LCD_SendCommand(
        LCD_CMD_ENTRY_MODE |
        LCD_ENTRY_INCREMENT
    );

    return LCD_OK;
}

/*----------------------------------------------------------
 * LCD_SendCommand
 *----------------------------------------------------------*/

void LCD_SendCommand(uint8_t command)
{
    LCD_RS_Low();

    LCD_WriteByte(command);

    /*
     * Commands such as CLEAR and HOME require
     * longer execution time.
     */

    if ((command == LCD_CMD_CLEAR) ||
        (command == LCD_CMD_HOME))
    {
        LCD_Delay(50000U);
    }
    else
    {
        LCD_Delay(2000U);
    }
}

/*----------------------------------------------------------
 * LCD_SendData
 *----------------------------------------------------------*/

void LCD_SendData(uint8_t data)
{
    LCD_RS_High();

    LCD_WriteByte(data);

    LCD_Delay(2000U);
}

/*----------------------------------------------------------
 * LCD_Clear
 *----------------------------------------------------------*/

void LCD_Clear(void)
{
    LCD_SendCommand(LCD_CMD_CLEAR);
}

/*----------------------------------------------------------
 * LCD_Home
 *----------------------------------------------------------*/

void LCD_Home(void)
{
    LCD_SendCommand(LCD_CMD_HOME);
}

/*----------------------------------------------------------
 * LCD_SetCursor
 *----------------------------------------------------------*/

LCD_Status_t LCD_SetCursor(uint8_t row,
                           uint8_t column)
{
    uint8_t address;

    if (row >= LCD_ROWS)
    {
        return LCD_INVALID_PARAMETER;
    }

    if (column >= LCD_COLUMNS)
    {
        return LCD_INVALID_PARAMETER;
    }

    /*
     * Standard HD44780 DDRAM addresses:
     *
     * Row 0 -> 0x00
     * Row 1 -> 0x40
     * Row 2 -> 0x14
     * Row 3 -> 0x54
     */

    switch (row)
    {
        case 0:
            address = 0x00U;
            break;

        case 1:
            address = 0x40U;
            break;

        case 2:
            address = 0x14U;
            break;

        case 3:
            address = 0x54U;
            break;

        default:
            return LCD_INVALID_PARAMETER;
    }

    address += column;

    LCD_SendCommand(
        LCD_CMD_SET_DDRAM | address
    );

    return LCD_OK;
}

/*----------------------------------------------------------
 * LCD_PutChar
 *----------------------------------------------------------*/

void LCD_PutChar(char character)
{
    LCD_SendData(
        (uint8_t)character
    );
}

/*----------------------------------------------------------
 * LCD_Print
 *----------------------------------------------------------*/

void LCD_Print(const char *string)
{
    if (string == NULL)
    {
        return;
    }

    while (*string != '\0')
    {
        LCD_PutChar(*string);

        string++;
    }
}

/*----------------------------------------------------------
 * LCD_PrintAt
 *----------------------------------------------------------*/

LCD_Status_t LCD_PrintAt(uint8_t row,
                         uint8_t column,
                         const char *string)
{
    LCD_Status_t status;

    if (string == NULL)
    {
        return LCD_INVALID_PARAMETER;
    }

    status = LCD_SetCursor(row, column);

    if (status != LCD_OK)
    {
        return status;
    }

    LCD_Print(string);

    return LCD_OK;
}

/*----------------------------------------------------------
 * LCD_PrintInt
 *----------------------------------------------------------*/

void LCD_PrintInt(int32_t value)
{
    char buffer[12];

    uint32_t index = 0U;

    uint32_t number;

    if (value < 0)
    {
        LCD_PutChar('-');

        /*
         * Convert negative number safely.
         */

        number = (uint32_t)(-(value + 1));
        number += 1U;
    }
    else
    {
        number = (uint32_t)value;
    }

    if (number == 0U)
    {
        LCD_PutChar('0');
        return;
    }

    while (number > 0U)
    {
        buffer[index++] =
            (char)('0' + (number % 10U));

        number /= 10U;
    }

    while (index > 0U)
    {
        LCD_PutChar(buffer[--index]);
    }
}

/*----------------------------------------------------------
 * LCD_PrintUInt
 *----------------------------------------------------------*/

void LCD_PrintUInt(uint32_t value)
{
    char buffer[11];

    uint32_t index = 0U;

    if (value == 0U)
    {
        LCD_PutChar('0');
        return;
    }

    while (value > 0U)
    {
        buffer[index++] =
            (char)('0' + (value % 10U));

        value /= 10U;
    }

    while (index > 0U)
    {
        LCD_PutChar(buffer[--index]);
    }
}

/*----------------------------------------------------------
 * LCD_DisplayOn
 *----------------------------------------------------------*/

void LCD_DisplayOn(void)
{
    LCD_SendCommand(
        LCD_CMD_DISPLAY_CTRL |
        LCD_DISPLAY_ON |
        LCD_CURSOR_OFF |
        LCD_BLINK_OFF
    );
}

/*----------------------------------------------------------
 * LCD_DisplayOff
 *----------------------------------------------------------*/

void LCD_DisplayOff(void)
{
    LCD_SendCommand(
        LCD_CMD_DISPLAY_CTRL |
        LCD_DISPLAY_OFF
    );
}

/*----------------------------------------------------------
 * LCD_CursorOn
 *----------------------------------------------------------*/

void LCD_CursorOn(void)
{
    LCD_SendCommand(
        LCD_CMD_DISPLAY_CTRL |
        LCD_DISPLAY_ON |
        LCD_CURSOR_ON |
        LCD_BLINK_OFF
    );
}

/*----------------------------------------------------------
 * LCD_CursorOff
 *----------------------------------------------------------*/

void LCD_CursorOff(void)
{
    LCD_SendCommand(
        LCD_CMD_DISPLAY_CTRL |
        LCD_DISPLAY_ON |
        LCD_CURSOR_OFF |
        LCD_BLINK_OFF
    );
}

/*----------------------------------------------------------
 * LCD_BlinkOn
 *----------------------------------------------------------*/

void LCD_BlinkOn(void)
{
    LCD_SendCommand(
        LCD_CMD_DISPLAY_CTRL |
        LCD_DISPLAY_ON |
        LCD_CURSOR_ON |
        LCD_BLINK_ON
    );
}

/*----------------------------------------------------------
 * LCD_BlinkOff
 *----------------------------------------------------------*/

void LCD_BlinkOff(void)
{
    LCD_SendCommand(
        LCD_CMD_DISPLAY_CTRL |
        LCD_DISPLAY_ON |
        LCD_CURSOR_ON |
        LCD_BLINK_OFF
    );
}
