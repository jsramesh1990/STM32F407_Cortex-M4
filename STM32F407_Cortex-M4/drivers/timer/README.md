timer fits your Scrambler project

This driver is particularly useful for measuring the anagram search performance.

For example in scrambler.c:

uint32_t start;
uint32_t elapsed;


start = Timer_GetTimestamp();


/* Search dictionary */
Scrambler_FindMatches(input);


elapsed = Timer_GetElapsedUs(start);


LCD_PrintAt(1, 0, "Time: ");
LCD_PrintUInt(elapsed);
LCD_Print(" us");

You can then demonstrate in an interview:

User enters "stpo"
        │
        ▼
Input validation
        │
        ▼
Dictionary search
        │
        ▼
Anagram comparison
        │
        ▼
Results generated
        │
        ▼
Timer measures execution time
        │
        ▼
UART / LCD
One important hardware assumption

The above implementation assumes TIM2's effective timer clock is 84 MHz, which is the normal STM32F407 setup when APB1 timer clocks are 84 MHz. Your clock.c must actually configure the MCU that way.

If your clock.c uses a different clock tree, the value:

#define TIMER_INPUT_CLOCK_HZ 84000000UL

must be changed accordingly.

Your driver tree is now:

drivers/
├── flash/
│   ├── flash.c
│   └── flash.h
├── gpio/
│   ├── gpio.c
│   └── gpio.h
├── lcd/
│   ├── lcd.c
│   └── lcd.h
├── spi/
│   ├── spi.c
│   └── spi.h
├── timer/
│   ├── timer.c
│   └── timer.h
└── uart/
    ├── uart.c
    └── uart.h

The next file pair in this architecture is uart.c and uart.h, which will provide the command-line interface for entering the scrambled word and displaying the matching words.
