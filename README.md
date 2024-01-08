# Digital Stopwatch with Lap Function

![STM32](https://img.shields.io/badge/STM32-Development%20Board-blue)
![C](https://img.shields.io/badge/Language-C-green)
![LCD](https://img.shields.io/badge/LCD-Display-informational)

## Overview

A simple digital stopwatch with a lap function designed for the UCT STM32 Development Board using the STM32F0's General-purpose timer unit TIM14. This stopwatch provides basic timing functionalities and lap tracking with visual indicators using LEDs.

## Features

- **Start (SW0):** Initiates the stopwatch, counting up from 00:00.00 in minutes:seconds.hundredths.
  
- **Lap (SW1):** Freezes display on the current count value while counting continues in the background. Returns to the standard stopwatch display upon pressing START, LAP, STOP, or RESET.

- **Stop (SW2):** Halts counting, holds the current time value, and displays it until RESET is pressed or START is pressed to resume counting from the previous value.

- **Reset (SW3):** Resets the stopwatch count to 00:00.00 and displays the welcome message.

## LCD Display

Displays real-time stopwatch information and messages, including the start message, current time, and lap display.

## LED Indicators

- (D0) = START
- (D1) = LAP
- (D2) = STOP
- (D3) = RESET

## How to Use

1. Power up the STM32 development board.

2. The LCD will show the initial message: "Stop Watch Press SW0..."

3. Press SW0 to start the stopwatch.

4. Press SW1 to freeze the display on the current count value (lap function).

5. Press SW2 to stop counting and display the current time.

6. Press SW3 to reset the stopwatch count and display the welcome message.

## LED Status

- (D0) ON: Stopwatch is in the START state.
- (D1) ON: Stopwatch is in the LAP state.
- (D2) ON: Stopwatch is in the STOP state.
- (D3) ON: Stopwatch is in the RESET state.

## Implementation

The program is written in C and utilizes the STM32F0's TIM14 for timer functionality. LCD is used for display, and LEDs indicate the current state.

## Acknowledgments

- Practical Manual: [EE2046F/EE2050F](link-to-manual)
- STM32 Documentation: [STMicroelectronics](link-to-docs)

---

**Note:** Replace placeholders like `[STM32](https://img.shields.io/badge/STM32-Development%20Board-blue)` with actual links to relevant resources. Provide links to the practical manual and STM32 documentation if applicable.
