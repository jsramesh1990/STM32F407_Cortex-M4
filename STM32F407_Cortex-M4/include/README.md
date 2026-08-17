And the overall architecture becomes:

                         main.c
                           │
                           ▼
                        main.h
                           │
             ┌─────────────┼─────────────┐
             │             │             │
             ▼             ▼             ▼
         config.h       types.h       error.h
             │
             ▼
        APPLICATION
             │
     ┌───────┼────────┐
     │       │        │
     ▼       ▼        ▼
scrambler dictionary result_manager
     │       │        │
     └───────┼────────┘
             ▼
          DRIVERS
             │
   ┌─────────┼──────────┐
   │         │          │
 UART       SPI        GPIO
   │         │          │
   │       FLASH       LCD
   │
   └─────────┬──────────┘
             ▼
            HAL
             │
           CMSIS
             │
             ▼
      STM32F407 Cortex-M4
One correction to keep your project clean

Because your project already has:

cmsis/STM32F4xx/Include/
    stm32f407xx.h
    stm32f4xx.h
    core_cm4.h
    ...

Do not create another stm32f407xx.h inside include/. Your main.h should include the CMSIS version:

#include "stm32f407xx.h"

with the compiler include path pointing to:

cmsis/STM32F4xx/Include

Similarly, your driver include paths should expose:

include/
bsp/
hal/
drivers/gpio/
drivers/uart/
drivers/spi/
drivers/flash/
drivers/lcd/
drivers/timer/
application/
cmsis/STM32F4xx/Include/

That gives you a proper STM32F407 embedded project structure, rather than just a PC C application moved onto an MCU.
