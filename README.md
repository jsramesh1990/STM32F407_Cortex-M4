# STM32F407 Cortex-M4 — Scrambler Word Solver

[![Language](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Processor](https://img.shields.io/badge/Processor-STM32F407VG-green.svg)](https://www.st.com/en/microcontrollers-microprocessors/stm32f407-417.html)
[![Core](https://img.shields.io/badge/Core-ARM%20Cortex--M4-orange.svg)](https://developer.arm.com/Processors/Cortex-M4)
[![Architecture](https://img.shields.io/badge/Architecture-ARM%20v7E--M-purple.svg)](https://developer.arm.com/architectures/cpu-architecture/m-profile)
[![Flash](https://img.shields.io/badge/Flash-1MB-yellow.svg)](https://www.st.com/en/microcontrollers-microprocessors/stm32f407-417.html)
[![SRAM](https://img.shields.io/badge/SRAM-128KB-red.svg)](https://www.st.com/en/microcontrollers-microprocessors/stm32f407-417.html)
[![CMSIS](https://img.shields.io/badge/CMSIS-STM32F4xx-lightgrey.svg)](https://arm-software.github.io/CMSIS_6/latest/)
[![Build](https://img.shields.io/badge/Build-Makefile-success.svg)](https://www.gnu.org/software/make/)
[![Compiler](https://img.shields.io/badge/Compiler-arm--none--eabi--gcc-blue.svg)](https://developer.arm.com/downloads/-/gnu-rm)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Embedded%20Systems-orange.svg)](https://www.st.com/)
[![Status](https://img.shields.io/badge/Status-Active-brightgreen.svg)]()

---

## Table of Contents

- [Project Overview](#project-overview)
- [Project Objectives](#project-objectives)
- [Target Hardware](#target-hardware)
- [Processor Architecture](#processor-architecture)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Project Directory Structure](#project-directory-structure)
- [Software Layers](#software-layers)
- [Application Flow](#application-flow)
- [Scrambler Algorithm](#scrambler-algorithm)
- [Dictionary Processing](#dictionary-processing)
- [UART Communication](#uart-communication)
- [Middleware](#middleware)
- [Drivers](#drivers)
- [Memory Architecture](#memory-architecture)
- [Linker Script](#linker-script)
- [Startup Flow](#startup-flow)
- [Build System](#build-system)
- [Building the Project](#building-the-project)
- [Running Tests](#running-tests)
- [Firmware Outputs](#firmware-outputs)
- [Programming the STM32](#programming-the-stm32)
- [Debugging](#debugging)
- [Testing Strategy](#testing-strategy)
- [Future Improvements](#future-improvements)
- [Embedded Software Concepts Demonstrated](#embedded-software-concepts-demonstrated)
- [Interview Explanation](#interview-explanation)
- [License](#license)

---

# Project Overview

**STM32F407 Cortex-M4 — Scrambler Word Solver** is an embedded C application designed for the **STM32F407VG microcontroller**.

The project receives a scrambled word through a communication interface such as UART, validates the input, compares it against a dictionary stored in external/non-volatile memory, and returns all valid English anagrams.

### Example

Input:

```text
stpo
````

Possible results:

```text
stop
pots
post
tops
spot
```

Another example:

```text
ew
```

Result:

```text
we
```

The project demonstrates how a normal C application can be transformed into a structured **embedded firmware architecture** consisting of:

```text
Application
     ↓
Middleware
     ↓
Drivers
     ↓
HAL
     ↓
CMSIS
     ↓
STM32F407 Cortex-M4
     ↓
Hardware
```

---

# Project Objectives

The main objectives of this project are:

* Develop an embedded C application for STM32F407.
* Implement an anagram/scrambled-word solving algorithm.
* Interface with UART for user input and output.
* Implement reusable middleware components.
* Implement GPIO, SPI, Flash, LCD and Timer drivers.
* Use CMSIS for Cortex-M4 and STM32 register definitions.
* Use a custom linker script.
* Implement startup and reset handling.
* Demonstrate embedded memory management.
* Implement ring-buffer based communication.
* Implement fixed-size memory-pool management.
* Separate application, middleware and hardware-dependent code.
* Generate production firmware images.
* Provide unit tests for application and middleware components.

---

# Target Hardware

## STM32F407VG

The primary target processor is:

| Parameter             | Specification       |
| --------------------- | ------------------- |
| MCU                   | STM32F407VG         |
| CPU                   | ARM Cortex-M4       |
| Architecture          | ARMv7E-M            |
| Maximum CPU Frequency | 168 MHz             |
| Flash                 | 1 MB                |
| SRAM                  | 128 KB              |
| CCM RAM               | 64 KB               |
| FPU                   | Single Precision    |
| DSP Instructions      | Yes                 |
| DMA                   | Yes                 |
| GPIO                  | Multiple GPIO Ports |
| UART/USART            | Multiple interfaces |
| SPI                   | Multiple interfaces |
| Timers                | Multiple timers     |
| Debug                 | SWD/JTAG            |

---

# Processor Architecture

The firmware executes on the:

```text
ARM Cortex-M4
```

The Cortex-M4 provides:

* 32-bit ARM architecture
* Thumb-2 instruction set
* DSP instructions
* Hardware multiply/divide
* Floating Point Unit
* Nested Vectored Interrupt Controller
* SysTick timer
* Memory protection support
* Efficient interrupt handling

The processor starts execution from:

```text
0x08000000
```

where the interrupt vector table is normally located.

---

# Features

## Application

* Scrambled word validation
* Dictionary processing
* Anagram matching
* Result management
* User input management
* Word length validation
* Alphabetic character validation

## Middleware

* Ring buffer
* Memory pool
* String utilities

## Drivers

* UART
* SPI
* Flash
* GPIO
* LCD
* Timer

## BSP

* Board initialization
* Clock initialization

## Low-Level Software

* CMSIS
* Startup assembly
* Linker script
* HAL abstraction

---

# System Architecture

```text
                    ┌──────────────────────────────┐
                    │          USER / PC           │
                    │        UART TERMINAL         │
                    └──────────────┬───────────────┘
                                   │
                                   ▼
                    ┌──────────────────────────────┐
                    │       INPUT MANAGER          │
                    │                              │
                    │ Validation / Input Handling  │
                    └──────────────┬───────────────┘
                                   │
                                   ▼
                    ┌──────────────────────────────┐
                    │        SCRAMBLER             │
                    │                              │
                    │ Anagram Search Algorithm     │
                    └──────────────┬───────────────┘
                                   │
                     ┌─────────────┴─────────────┐
                     │                           │
                     ▼                           ▼
          ┌───────────────────┐       ┌───────────────────┐
          │    DICTIONARY     │       │  RESULT MANAGER   │
          │                   │       │                   │
          │ Word Database     │       │ Result Handling   │
          └─────────┬─────────┘       └─────────┬─────────┘
                    │                           │
                    ▼                           │
          ┌───────────────────┐                 │
          │    MIDDLEWARE     │◄────────────────┘
          │                   │
          │ String Utilities  │
          │ Ring Buffer       │
          │ Memory Pool       │
          └─────────┬─────────┘
                    │
                    ▼
          ┌─────────────────────────────┐
          │           DRIVERS           │
          │                             │
          │ UART | SPI | FLASH | GPIO   │
          │ LCD  | TIMER                │
          └─────────────┬───────────────┘
                        │
                        ▼
          ┌─────────────────────────────┐
          │             HAL             │
          └─────────────┬───────────────┘
                        │
                        ▼
          ┌─────────────────────────────┐
          │           CMSIS             │
          │      STM32F4xx / M4         │
          └─────────────┬───────────────┘
                        │
                        ▼
          ┌─────────────────────────────┐
          │     STM32F407 Cortex-M4     │
          └─────────────────────────────┘
```

---

# Project Directory Structure

```text
STM32F407_Cortex-M4/
│
├── application/
│   ├── scrambler.c
│   ├── scrambler.h
│   ├── input_manager.c
│   ├── input_manager.h
│   ├── dictionary.c
│   ├── dictionary.h
│   ├── result_manager.c
│   └── result_manager.h
│
├── bsp/
│   ├── board.c
│   ├── board.h
│   ├── clock.c
│   └── clock.h
│
├── build/
│   ├── obj/
│   ├── scrambler_stm32.elf
│   ├── scrambler_stm32.hex
│   ├── scrambler_stm32.bin
│   ├── scrambler_stm32.map
│   └── README.md
│
├── cmsis/
│   └── STM32F4xx/
│       ├── Include/
│       │   ├── stm32f407xx.h
│       │   ├── stm32f4xx.h
│       │   ├── core_cm4.h
│       │   ├── core_cmFunc.h
│       │   ├── core_cmInstr.h
│       │   ├── core_cmSimd.h
│       │   └── system_stm32f4xx.h
│       │
│       └── Source/
│           └── system_stm32f4xx.c
│
├── data/
│   ├── words.txt
│   └── words.bin
│
├── docs/
│   ├── architecture.md
│   ├── memory_map.md
│   ├── testing.md
│   └── uart_protocol.md
│
├── drivers/
│   ├── flash/
│   │   ├── flash.c
│   │   └── flash.h
│   │
│   ├── gpio/
│   │   ├── gpio.c
│   │   └── gpio.h
│   │
│   ├── lcd/
│   │   ├── lcd.c
│   │   └── lcd.h
│   │
│   ├── spi/
│   │   ├── spi.c
│   │   └── spi.h
│   │
│   ├── timer/
│   │   ├── timer.c
│   │   └── timer.h
│   │
│   └── uart/
│       ├── uart.c
│       └── uart.h
│
├── hal/
│   ├── stm32f4xx_hal.c
│   └── stm32f4xx_hal.h
│
├── include/
│   ├── config.h
│   ├── error.h
│   ├── main.h
│   └── types.h
│
├── linker/
│   └── STM32F407VG_FLASH.ld
│
├── middleware/
│   ├── memory_pool.c
│   ├── memory_pool.h
│   ├── ring_buffer.c
│   ├── ring_buffer.h
│   ├── string_utils.c
│   └── string_utils.h
│
├── startup/
│   └── startup_stm32f407xx.s
│
├── tests/
│   ├── test_dictionary.c
│   ├── test_input.c
│   ├── test_scrambler.c
│   └── test_string_utils.c
│
├── details.md
├── Makefile
├── README.md
└── tree
```

---

# Software Layers

The project follows a layered embedded software architecture.

## 1. Application Layer

```text
application/
```

Contains application-specific functionality.

```text
scrambler.c
dictionary.c
input_manager.c
result_manager.c
```

This layer should not directly manipulate STM32 registers.

---

## 2. Middleware Layer

```text
middleware/
```

Provides reusable software services.

### Ring Buffer

Used for asynchronous UART communication.

```text
UART RX
   ↓
Ring Buffer
   ↓
Input Manager
```

### Memory Pool

Provides deterministic fixed-size memory allocation.

```text
Application
     ↓
Memory Pool
     ↓
Fixed memory blocks
```

### String Utilities

Provides:

* String length
* String copy
* Case conversion
* Alphabet validation
* String sorting
* Anagram comparison
* String trimming

---

## 3. Driver Layer

```text
drivers/
```

Provides hardware-specific peripheral drivers.

```text
UART
SPI
FLASH
GPIO
LCD
TIMER
```

---

## 4. BSP Layer

```text
bsp/
```

Board-specific initialization.

```text
board.c
clock.c
```

---

## 5. HAL

```text
hal/
```

Provides an abstraction between the application/driver code and MCU hardware.

---

## 6. CMSIS

```text
cmsis/
```

Provides ARM Cortex-M and STM32 device definitions.

Examples:

```text
stm32f407xx.h
core_cm4.h
system_stm32f4xx.h
```

---

# Application Flow

The complete runtime flow is:

```text
Reset
  ↓
Startup Assembly
  ↓
SystemInit()
  ↓
main()
  ↓
Board Initialization
  ↓
Clock Initialization
  ↓
Peripheral Initialization
  ↓
Dictionary Initialization
  ↓
UART Initialization
  ↓
Application Loop
  ↓
Receive Scrambled Word
  ↓
Validate Input
  ↓
Sort Characters
  ↓
Search Dictionary
  ↓
Compare Sorted Words
  ↓
Store Matching Results
  ↓
Send Results Through UART
  ↓
Wait For Next Input
```

---

# Scrambler Algorithm

The core algorithm uses character sorting.

For example:

```text
Input:

stpo
```

Sort the characters:

```text
stpo
 ↓
opst
```

Dictionary words are processed similarly.

```text
stop
 ↓
opst
```

```text
pots
 ↓
opst
```

```text
post
 ↓
opst
```

```text
tops
 ↓
opst
```

Because the sorted strings are identical:

```text
stpo → opst

stop → opst
pots → opst
post → opst
tops → opst
```

all four are valid anagrams.

---

# Dictionary Processing

The dictionary is provided as:

```text
data/
├── words.txt
└── words.bin
```

The text dictionary can contain entries such as:

```text
stop
pots
post
tops
spot
hello
world
```

The firmware validates words before processing.

Invalid entries may include:

```text
abc123
hello!
12345
```

Supported word length:

```text
2 - 45 characters
```

---

# UART Communication

UART is used as the primary user interface.

Example:

```text
PC Terminal
     │
     │  stpo
     ▼
STM32F407
     │
     ▼
Scrambler
     │
     ▼
UART
     │
     ▼
PC Terminal
```

Example terminal session:

```text
================================
 STM32F407 Scrambler Solver
================================

Enter scrambled word: stpo

Matches:

stop
pots
post
tops
spot
```

Recommended UART configuration:

| Parameter    |  Value |
| ------------ | -----: |
| Baud Rate    | 115200 |
| Data Bits    |      8 |
| Stop Bits    |      1 |
| Parity       |   None |
| Flow Control |   None |

Configuration:

```text
115200 8N1
```

---

# Middleware

## Ring Buffer

The UART driver can place received bytes into a ring buffer.

```text
             UART RX
                │
                ▼
        ┌────────────────┐
        │  Ring Buffer   │
        │                │
        │ H →            │
        │ [s][t][p][o]   │
        │          ← T   │
        └────────┬───────┘
                 │
                 ▼
          Input Manager
```

Advantages:

* Non-blocking reception
* Interrupt-friendly
* Producer/consumer architecture
* Prevents immediate processing inside ISR

---

## Memory Pool

The memory pool provides deterministic allocation.

Instead of repeatedly calling:

```c
malloc();
free();
```

the application can allocate fixed-size blocks.

Advantages:

* Predictable memory usage
* Reduced fragmentation
* Faster allocation
* Suitable for embedded systems

---

## String Utilities

The project provides reusable string operations.

Examples:

```c
String_Length();

String_Copy();

String_ToLower();

String_ToUpper();

String_Sort();

String_IsAnagram();
```

---

# Drivers

## UART Driver

```text
drivers/uart/
```

Responsibilities:

* UART initialization
* Transmit byte
* Receive byte
* Transmit string
* Receive string
* UART status

---

## SPI Driver

```text
drivers/spi/
```

Responsibilities:

* SPI initialization
* SPI transmit
* SPI receive
* SPI full-duplex transfer

---

## Flash Driver

```text
drivers/flash/
```

Responsibilities:

* Flash initialization
* Flash read
* Flash write
* Flash erase
* Flash status

---

## GPIO Driver

```text
drivers/gpio/
```

Responsibilities:

* GPIO initialization
* GPIO read
* GPIO write
* GPIO toggle

---

## LCD Driver

```text
drivers/lcd/
```

Responsibilities:

* LCD initialization
* LCD command
* LCD data
* LCD text display
* LCD clear

---

## Timer Driver

```text
drivers/timer/
```

Responsibilities:

* Timer initialization
* Timer start
* Timer stop
* Delay
* Timeout generation

---

# Memory Architecture

The STM32F407VG memory map used by the project is:

```text
+--------------------------------------+
|              FLASH                   |
|                                      |
| 0x08000000                           |
|                                      |
| .isr_vector                          |
| .text                                |
| .rodata                              |
| Exception tables                     |
|                                      |
|              1 MB                    |
|                                      |
+--------------------------------------+
|              SRAM                    |
|                                      |
| 0x20000000                           |
|                                      |
| .data                                |
| .bss                                 |
| Heap                                 |
| Stack                                |
|                                      |
|             128 KB                   |
|                                      |
+--------------------------------------+
|             CCM RAM                  |
|                                      |
| 0x10000000                           |
|                                      |
| Fast application data               |
|                                      |
|              64 KB                   |
+--------------------------------------+
```

---

# Linker Script

The linker script is:

```text
linker/STM32F407VG_FLASH.ld
```

It defines:

* FLASH region
* SRAM region
* CCM RAM region
* `.text`
* `.rodata`
* `.data`
* `.bss`
* Heap
* Stack
* Interrupt vector table

Example:

```ld
MEMORY
{
    FLASH  (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM    (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
    CCMRAM (xrw) : ORIGIN = 0x10000000, LENGTH = 64K
}
```

---

# Startup Flow

The startup file is:

```text
startup/startup_stm32f407xx.s
```

The startup sequence is:

```text
Power On
   ↓
Reset_Handler
   ↓
Set Stack Pointer
   ↓
SystemInit()
   ↓
Copy .data
   ↓
Clear .bss
   ↓
C Runtime Initialization
   ↓
main()
```

The interrupt vector table contains addresses for:

```text
Reset_Handler
NMI_Handler
HardFault_Handler
MemManage_Handler
BusFault_Handler
UsageFault_Handler
SVC_Handler
DebugMon_Handler
PendSV_Handler
SysTick_Handler
```

and peripheral interrupt handlers.

---

# Build System

The project uses a Makefile:

```text
Makefile
```

Typical toolchain:

```text
arm-none-eabi-gcc
arm-none-eabi-objcopy
arm-none-eabi-size
arm-none-eabi-objdump
```

---

# Building the Project

## Install ARM GCC Toolchain

Ubuntu/Debian:

```bash
sudo apt update
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi
```

Verify:

```bash
arm-none-eabi-gcc --version
```

---

## Build

From the project root:

```bash
make
```

Clean the project:

```bash
make clean
```

Rebuild:

```bash
make clean
make
```

---

# Firmware Outputs

After a successful build:

```text
build/
├── obj/
├── scrambler_stm32.elf
├── scrambler_stm32.hex
├── scrambler_stm32.bin
└── scrambler_stm32.map
```

## ELF

```text
scrambler_stm32.elf
```

Used for:

* Debugging
* GDB
* Symbol information
* Debug probes

---

## HEX

```text
scrambler_stm32.hex
```

Used for:

* Flash programming
* Firmware distribution

---

## BIN

```text
scrambler_stm32.bin
```

Raw binary image.

Useful for:

* Bootloader flashing
* Production programming
* DFU workflows

---

## MAP

```text
scrambler_stm32.map
```

Used to inspect:

* Memory usage
* Symbols
* Section placement
* Function addresses
* RAM/Flash consumption

---

# Programming the STM32

The generated firmware can be programmed using an ST-LINK compatible debugger/programmer.

Typical flow:

```text
Build
  ↓
scrambler_stm32.elf
  ↓
ST-LINK
  ↓
SWD
  ↓
STM32F407
```

Typical programming interface:

```text
SWDIO
SWCLK
GND
3.3V
```

---

# Debugging

The project supports standard ARM Cortex-M debugging through SWD/JTAG.

Typical debugging flow:

```text
GDB
 │
 ▼
OpenOCD / ST-LINK
 │
 ▼
SWD
 │
 ▼
STM32F407
```

Useful debugging information is available in:

```text
build/scrambler_stm32.map
```

and:

```text
build/scrambler_stm32.elf
```

---

# Testing Strategy

Unit tests are located in:

```text
tests/
```

Current tests:

```text
test_dictionary.c
test_input.c
test_scrambler.c
test_string_utils.c
```

## String Tests

Tests:

* String length
* Alphabet validation
* Case conversion
* Sorting
* Anagram detection

---

## Input Tests

Tests:

* Valid words
* Invalid characters
* Numbers
* Special characters
* Minimum word length
* Uppercase input

---

## Scrambler Tests

Tests:

```text
stpo → stop
stpo → pots
stpo → post
stpo → tops
```

and negative cases:

```text
stpo != hello
```

---

## Dictionary Tests

Tests:

* Dictionary initialization
* Word insertion
* Word lookup
* Dictionary count
* Invalid entries

---

# Example Test Output

```text
========================================
 String Utilities Unit Tests
 STM32F407 Cortex-M4 Project
========================================

--- String Length Tests ---

[PASS] String_Length("hello") == 5U
[PASS] String_Length("") == 0U
[PASS] String_Length("STM32F407") == 9U

--- String Sort Tests ---

[PASS] strcmp(buffer, "opst") == 0

--- Anagram Tests ---

[PASS] String_IsAnagram("stpo", "stop") == 1U
[PASS] String_IsAnagram("stpo", "pots") == 1U
[PASS] String_IsAnagram("stpo", "post") == 1U

========================================
Tests Passed : 10
Tests Failed : 0
========================================
```

---

# Future Improvements

Possible future enhancements:

* Trie-based dictionary search
* Hash-table based dictionary lookup
* Binary-search dictionary
* Faster anagram signatures
* DMA-based UART
* Interrupt-driven UART
* SPI Flash dictionary storage
* External QSPI Flash
* LCD result display
* FreeRTOS integration
* Low-power mode
* Watchdog integration
* CRC validation
* Firmware versioning
* Bootloader support
* OTA firmware update
* Hardware-in-the-loop testing

---

# Performance Improvements

The initial implementation compares sorted strings.

For a larger dictionary, the following optimization can be introduced:

```text
Current:

Input
 ↓
Sort
 ↓
Compare against every dictionary word
 ↓
O(N × M log M)
```

Optimized architecture:

```text
Input
 ↓
Generate character signature
 ↓
Hash table lookup
 ↓
Return matching words
```

This significantly reduces lookup time for large dictionaries.

---

# Embedded Software Concepts Demonstrated

This project demonstrates several concepts commonly used in embedded software development.

### C Programming

* Pointers
* Arrays
* Structures
* Enumerations
* Function pointers
* Static functions
* Header/source separation
* Memory management

### Embedded Systems

* MCU startup
* Interrupt vector table
* Memory map
* Linker script
* Startup assembly
* Peripheral drivers
* BSP
* HAL
* CMSIS

### Communication

* UART
* SPI
* Ring buffers
* Interrupt-driven communication

### Memory

* Stack
* Heap
* `.text`
* `.rodata`
* `.data`
* `.bss`
* CCM RAM
* Memory pools

### Software Architecture

* Layered architecture
* Hardware abstraction
* Middleware
* Application separation
* Unit testing

---

# Interview Explanation

A concise interview explanation:

> **"I developed a Scrambler Word Solver on the STM32F407 Cortex-M4 using Embedded C. The firmware receives a scrambled word through UART, validates the input, generates a sorted character representation, and searches a dictionary for matching anagrams. I structured the firmware into application, middleware, BSP, driver, HAL and CMSIS layers. I implemented UART, SPI, Flash, GPIO, LCD and timer drivers, along with a ring buffer and fixed-size memory pool. The project also includes a custom linker script, Cortex-M4 startup code, unit tests and production ELF, HEX and BIN firmware outputs."**

---

# High-Level Firmware Flow

```text
                  STM32F407
                     │
                     ▼
                Reset_Handler
                     │
                     ▼
                 SystemInit()
                     │
                     ▼
                    main()
                     │
                     ▼
              Board Initialization
                     │
                     ▼
             Peripheral Initialization
                     │
          ┌──────────┼───────────┐
          │          │           │
         UART       SPI        TIMER
          │          │           │
          └──────────┼───────────┘
                     │
                     ▼
             Dictionary Init
                     │
                     ▼
             Wait for UART Input
                     │
                     ▼
              Input Validation
                     │
                     ▼
              Scrambler Engine
                     │
                     ▼
             Dictionary Search
                     │
                     ▼
              Result Manager
                     │
                     ▼
              UART / LCD Output
                     │
                     ▼
                 Repeat
```

---

# Repository Status

| Component           | Status      |
| ------------------- | ----------- |
| STM32F407 Cortex-M4 | Implemented |
| Application Layer   | Implemented |
| BSP                 | Implemented |
| UART Driver         | Implemented |
| SPI Driver          | Implemented |
| Flash Driver        | Implemented |
| GPIO Driver         | Implemented |
| LCD Driver          | Implemented |
| Timer Driver        | Implemented |
| Middleware          | Implemented |
| CMSIS               | Included    |
| Startup Code        | Included    |
| Linker Script       | Included    |
| Dictionary          | Included    |
| Unit Tests          | Included    |
| Makefile            | Included    |
| ELF Output          | Generated   |
| HEX Output          | Generated   |
| BIN Output          | Generated   |
| MAP Output          | Generated   |

---

# License

This project is released under the MIT License.

See:

```text
LICENSE
```

---

# Author

**Embedded Software / Firmware Development Project**

Target:

```text
STM32F407VG
ARM Cortex-M4
Embedded C
```

---

## GitHub Repository

Repository:

**Scrambler Word Solver — STM32F407 Cortex-M4**

[https://github.com/jsramesh1990/Scrambler_Word_Solver](https://github.com/jsramesh1990/Scrambler_Word_Solver)

---

# Final Architecture

```text
┌──────────────────────────────────────────────┐
│              USER APPLICATION                │
│                                              │
│ Scrambler │ Dictionary │ Input │ Results    │
└───────────────────────┬──────────────────────┘
                        │
                        ▼
┌──────────────────────────────────────────────┐
│                 MIDDLEWARE                   │
│                                              │
│ String Utils │ Ring Buffer │ Memory Pool    │
└───────────────────────┬──────────────────────┘
                        │
                        ▼
┌──────────────────────────────────────────────┐
│                   DRIVERS                    │
│                                              │
│ UART │ SPI │ FLASH │ GPIO │ LCD │ TIMER     │
└───────────────────────┬──────────────────────┘
                        │
                        ▼
┌──────────────────────────────────────────────┐
│                    BSP                       │
│                                              │
│ Board Initialization │ Clock Configuration  │
└───────────────────────┬──────────────────────┘
                        │
                        ▼
┌──────────────────────────────────────────────┐
│                     HAL                      │
└───────────────────────┬──────────────────────┘
                        │
                        ▼
┌──────────────────────────────────────────────┐
│                    CMSIS                     │
│              Cortex-M4 / STM32F4             │
└───────────────────────┬──────────────────────┘
                        │
                        ▼
┌──────────────────────────────────────────────┐
│             STM32F407VG MCU                  │
│                                              │
│ ARM Cortex-M4 │ 168 MHz │ 1 MB Flash        │
│ 128 KB SRAM   │ 64 KB CCM RAM               │
└──────────────────────────────────────────────┘
```

---

**Built with Embedded C for ARM Cortex-M4.**

````

### One important GitHub recommendation

Your current project is already structured well. I would make the repository root look exactly like:

```text
STM32F407_Cortex-M4/
├── application/
├── bsp/
├── build/
├── cmsis/
├── data/
├── docs/
├── drivers/
├── hal/
├── include/
├── linker/
├── middleware/
├── startup/
├── tests/
├── details.md
├── LICENSE
├── Makefile
├── README.md
└── tree
````

I would **keep `build/` in the repository only if you intentionally want to show example/generated artifacts**. For a professional firmware repository, generated `.elf`, `.hex`, `.bin`, `.map`, and `obj/` are usually better excluded with `.gitignore`, while documenting how to regenerate them with the Makefile.
