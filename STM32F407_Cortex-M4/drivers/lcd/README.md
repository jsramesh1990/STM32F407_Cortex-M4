LCD hardware connection

With the code above:

STM32F407                LCD
────────────────────────────────
PB0   ────────────────── RS
PB1   ────────────────── EN
PB2   ────────────────── D4
PB3   ────────────────── D5
PB4   ────────────────── D6
PB5   ────────────────── D7


GND   ────────────────── RW
GND   ────────────────── VSS
+5V   ────────────────── VDD
How it fits your Scrambler project

You can use the LCD to show the result locally:

+----------------+
| Scrambler      |
| Enter word:    |
+----------------+

Then:

Input:
stpo


LCD:
+----------------+
| stpo            |
| Searching...   |
+----------------+

And finally:

+----------------+
| Matches: 5     |
| stop spot ...  |
+----------------+

Your application flow becomes:

UART Input
    │
    ├───────────────┐
    ▼               ▼
Scrambler       LCD Display
    │
    ▼
Dictionary
    │
    ▼
Results
    │
    ├──────────────> UART
    │
    └──────────────> LCD

Note: The LCD is optional for the Scrambler project. The core functionality only requires UART + SPI + external Flash + GPIO.
