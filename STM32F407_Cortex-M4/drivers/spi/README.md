I'll use STM32F407 SPI1 with:

PA5 → SPI1_SCK
PA6 → SPI1_MISO
PA7 → SPI1_MOSI
PA4 → Flash CS   (GPIO, controlled by flash.c)

Put them here:

STM32F407_Cortex-M4/
└── drivers/
    └── spi/
        ├── spi.c
        └── spi.h


driver structure now

You now have:

STM32F407_Cortex-M4/
└── drivers/
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
    └── spi/
        ├── spi.c
        └── spi.h
Important connection

Your Flash driver now uses the SPI driver:

dictionary.c
     │
     ▼
  flash.c
     │
     ▼
   spi.c
     │
     ▼
 STM32F407 SPI1
     │
     ├── PA5 → SCK
     ├── PA6 → MISO
     └── PA7 → MOSI

while Flash chip select is:

flash.c
   │
   ▼
 GPIOA PA4
   │
   ▼
 Flash CS

So for your Scrambler project, the dictionary path can be:

words.txt
   │
   ▼
words.bin
   │
   ▼
External SPI Flash
   │
   │ SPI1
   ▼
STM32F407
   │
   ▼
flash.c
   │
   ▼
dictionary.c
   │
   ▼
scrambler.c
   │
   ▼
result_manager.c
   │
   ├── UART
   └── LCD

This is a solid embedded architecture for presenting the project in an interview.
