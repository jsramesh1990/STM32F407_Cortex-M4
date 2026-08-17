For your STM32F407 Cortex-M4 Scrambler Word Solver, we'll use USART2 as the UART interface.

UART2 hardware
STM32F407              USB-UART / Terminal
──────────────────────────────────────────
PA2  (USART2_TX) ────> RX
PA3  (USART2_RX) <──── TX
GND ────────────────── GND

Typical terminal configuration:

Baud Rate : 115200
Data      : 8 bits
Parity    : None
Stop      : 1
Flow Ctrl : None

Put these files in:

STM32F407_Cortex-M4/
└── drivers/
    └── uart/
        ├── uart.c
        └── uart.h


How UART fits your Scrambler project

Your application flow becomes:

                    STM32F407
                       │
                       │
                 ┌─────▼─────┐
                 │   UART2   │
                 └─────┬─────┘
                       │
                       ▼
               input_manager.c
                       │
                       ▼
                scrambler.c
                       │
             ┌─────────┴─────────┐
             ▼                   ▼
       dictionary.c        result_manager.c
             │                   │
             ▼                   ▼
       SPI Flash                UART
             │                   │
             ▼                   ▼
        words.bin            Terminal

For example, the PC terminal can show:

================================
     STM32 SCRAMBLER SOLVER
================================


Dictionary loaded: 11873 words


Enter scrambled word: stpo


Searching...


Matches found:
  stop
  spot
  post
  pots
  tops


Search time: 142 us


Enter scrambled word:
Your complete driver layer is now
drivers/
│
├── flash/
│   ├── flash.c
│   └── flash.h
│
├── gpio/
│   ├── gpio.c
│   └── gpio.h
│
├── lcd/
│   ├── lcd.c
│   └── lcd.h
│
├── spi/
│   ├── spi.c
│   └── spi.h
│
├── timer/
│   ├── timer.c
│   └── timer.h
│
└── uart/
    ├── uart.c
    └── uart.h

One thing to keep consistent in your project: the current UART driver assumes APB1 = 42 MHz and the timer driver assumes TIM2 timer clock = 84 MHz. Those values should match the actual clock configuration in your bsp/clock.c; otherwise UART baud rate and timer timing will be incorrect.
