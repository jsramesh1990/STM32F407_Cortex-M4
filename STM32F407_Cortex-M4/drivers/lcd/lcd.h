#ifndef LCD_H
#define LCD_H

#include <stdint.h>

/*
 * HD44780-compatible LCD driver
 *
 * STM32F407 Cortex-M4
 *
 * Interface:
 *
 * LCD D4 -> STM32 GPIO
 * LCD D5 -> STM32 GPIO
 * LCD D6 -> STM32 GPIO
 * LCD D7 -> STM32 GPIO
 * LCD RS -> STM32 GPIO
 * LCD EN -> STM32 GPIO
 *
 * RW is connected to GND.
 */

/*----------------------------------------------------------
 * LCD Configuration
 *----------------------------------------------------------*/

#define LCD_COLUMNS             16U
#define LCD_ROWS                2U

/*----------------------------------------------------------
 * LCD Commands
 *----------------------------------------------------------*/

#define LCD_CMD_CLEAR           0x01U
#define LCD_CMD_HOME            0x02U

#define LCD_CMD_ENTRY_MODE      0x04U
#define LCD_CMD_DISPLAY_CTRL    0x08U
#define LCD_CMD_CURSOR_SHIFT    0x10U
#define LCD_CMD_FUNCTION_SET    0x20U
#define LCD_CMD_SET_CGRAM       0x40U
#define LCD_CMD_SET_DDRAM       0x80U

/* Entry mode flags */

#define LCD_ENTRY_INCREMENT     0x02U
#define LCD_ENTRY_DECREMENT     0x00U

/* Display control flags */

#define LCD_DISPLAY_ON          0x04U
#define LCD_DISPLAY_OFF         0x00U

#define LCD_CURSOR_ON           0x02U
#define LCD_CURSOR_OFF          0x00U

#define LCD_BLINK_ON            0x01U
#define LCD_BLINK_OFF           0x00U

/* Function set */

#define LCD_8_BIT_MODE          0x10U
#define LCD_4_BIT_MODE          0x00U

#define LCD_2_LINE              0x08U
#define LCD_1_LINE              0x00U

#define LCD_5X8_DOTS            0x00U
#define LCD_5X10_DOTS           0x04U

/*----------------------------------------------------------
 * LCD Status
 *----------------------------------------------------------*/

typedef enum
{
    LCD_OK = 0,
    LCD_ERROR,
    LCD_INVALID_PARAMETER

} LCD_Status_t;

/*----------------------------------------------------------
 * Initialization
 *----------------------------------------------------------*/

/*
 * Initialize LCD GPIOs and LCD controller.
 */
LCD_Status_t LCD_Init(void);

/*----------------------------------------------------------
 * Low-Level Functions
 *----------------------------------------------------------*/

/*
 * Send command to LCD.
 */
void LCD_SendCommand(uint8_t command);

/*
 * Send data/character to LCD.
 */
void LCD_SendData(uint8_t data);

/*----------------------------------------------------------
 * Display Functions
 *----------------------------------------------------------*/

/*
 * Clear LCD display.
 */
void LCD_Clear(void);

/*
 * Return cursor to home position.
 */
void LCD_Home(void);

/*
 * Set cursor position.
 *
 * row    = 0 to LCD_ROWS-1
 * column = 0 to LCD_COLUMNS-1
 */
LCD_Status_t LCD_SetCursor(uint8_t row,
                           uint8_t column);

/*
 * Print a single character.
 */
void LCD_PutChar(char character);

/*
 * Print a null-terminated string.
 */
void LCD_Print(const char *string);

/*
 * Print string at specified position.
 */
LCD_Status_t LCD_PrintAt(uint8_t row,
                         uint8_t column,
                         const char *string);

/*
 * Display integer value.
 */
void LCD_PrintInt(int32_t value);

/*
 * Display unsigned integer value.
 */
void LCD_PrintUInt(uint32_t value);

/*----------------------------------------------------------
 * Display Control
 *----------------------------------------------------------*/

/*
 * Turn display ON.
 */
void LCD_DisplayOn(void);

/*
 * Turn display OFF.
 */
void LCD_DisplayOff(void);

/*
 * Turn cursor ON.
 */
void LCD_CursorOn(void);

/*
 * Turn cursor OFF.
 */
void LCD_CursorOff(void);

/*
 * Turn cursor blinking ON.
 */
void LCD_BlinkOn(void);

/*
 * Turn cursor blinking OFF.
 */
void LCD_BlinkOff(void);

#endif /* LCD_H */
