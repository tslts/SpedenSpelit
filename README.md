# Spede's Games – Arduino Game

## Project Description

This project implements a reaction and memory game inspired by Spede's Games using Arduino. The objective is to observe a sequence of LED lights and reproduce it by pressing the correct buttons in the correct order.

As the game progresses, the sequence becomes faster, requiring increasingly better memory and reaction speed. The game ends if the player presses the wrong button, presses a button too early, or reaches the end of the sequence.

The project provides hands-on experience with microcontroller programming, interrupts, timers, digital output control, and modular software design.

## Features

- Four LEDs for displaying the sequence.
- Four buttons for player input.
- Two-digit seven-segment display for showing the score.
- Randomly generated LED sequences.
- Timer-based sequence control.
- Increasing game speed as the player progresses.
- Software-based button debouncing.
- Power-saving mode activated after 20 seconds of inactivity.
- Serial output for monitoring game events.

## Hardware Components

| Component | Purpose |
| --- | --- |
| Arduino-compatible microcontroller | Controls the game |
| 4 LEDs | Display the game sequence |
| 4 push buttons | Player input |
| Seven-segment display | Displays the score and countdown |
| Shift registers | Control the display segments |
| Resistors and jumper wires | Electrical connections |

**Note:** The display is controlled using shift registers. The exact wiring depends on the hardware being used.

## Pin Configuration

### LEDs

| LED | Arduino Pin |
| --- | --- |
| LED 0 | A2 |
| LED 1 | A3 |
| LED 2 | A4 |
| LED 3 | A5 |

The LEDs use active-LOW logic:

- `LOW` turns an LED on.
- `HIGH` turns an LED off.

### Buttons

| Button | Arduino Pin |
| --- | --- |
| Button 0 | D2 |
| Button 1 | D3 |
| Button 2 | D4 |
| Button 3 | D5 |

The buttons use the `INPUT_PULLUP` configuration. Pressing a button connects the pin to ground, causing its input value to become `LOW`.

Button state changes are detected using a pin change interrupt on Port D.

### Seven-Segment Display

| Signal | Arduino Pin |
| --- | --- |
| Reset | D12 |
| Shift Clock | D11 |
| Latch Clock | D10 |
| Output Enable | D9 |
| Serial Input | D8 |

The display is controlled by shifting segment bit patterns into shift registers. The program supports digits from 0 to 9 and displays values using two digit positions.

## How the Game Works

1. **Idle state:** The LEDs perform an introductory light show.
2. **Starting the game:** The player starts a new game by pressing the designated start button.
3. **Countdown:** The display counts down from 3 to 0.
4. **Sequence display:** The LEDs light up in a randomly generated sequence.
5. **Player's turn:** The player reproduces the sequence by pressing the corresponding buttons in the correct order.
6. **Scoring:** Each correct button press increases the score.
7. **Increasing difficulty:** The sequence playback becomes faster as the game progresses.
8. **Game over:** An incorrect button press or a premature guess ends the game.
9. **Restart:** The player can start a new round.

The sequence is stored in an array with a maximum capacity of 200 entries.

## Software Architecture

The program is divided into modules to separate hardware control from the main game logic.

| File | Responsibility |
| --- | --- |
| `SpedenSpelit.ino` | Main program, game states, scoring, and timer handling |
| `leds.h` / `leds.cpp` | LED control and light show routines |
| `buttons.h` / `buttons.cpp` | Button initialization and interrupts |
| `display.h` / `display.cpp` | Seven-segment display control |

These filenames describe the intended module responsibilities. Update this section if your actual repository uses different filenames or a different file structure.

### Key Functions

- `initializeLeds()` – Initializes the LED pins.
- `setLed()` – Turns on the selected LED and turns off the others.
- `clearAllLeds()` – Turns off all LEDs.
- `setAllLeds()` – Turns on all LEDs.
- `ledShow1()` – Runs the idle-state light show.
- `ledShow2()` – Runs an accelerating light show.
- `initButtonsAndButtonInterrupts()` – Initializes the buttons and interrupts.
- `initializeDisplay()` – Initializes the display control.
- `showResult()` – Displays a number on the display.
- `clearDisplay()` – Clears the display.
- `initializeTimer()` – Configures the Timer1 hardware timer.
- `createSequence()` – Generates a random sequence.
- `startGame()` – Starts a new round.
- `initializeGame()` – Resets the game state.
- `checkGame()` – Checks whether the player's button press is correct.

## Technical Implementation

### Timer and Interrupts

The LED sequence is controlled using the Timer1 hardware timer on an ATmega-based Arduino board. The timer operates in CTC mode with a prescaler of 1024.

At a clock frequency of 16 MHz, the initial comparison value for a one-second interval is:

```
OCR1A = 15624;
```

The timer interrupt service routine sets the `newTimerInterrupt` flag. The main game logic processes this flag inside the `loop()` function.

### Button Debouncing

Mechanical button bounce is filtered using a software-based approach. A 300-millisecond interval is applied between accepted interrupts, and additional interrupts within that interval are ignored.

### Increasing Difficulty

The game becomes faster as the sequence progresses. The timer comparison value is reduced after every ten sequence steps, increasing the playback speed.

### Power-Saving Mode

If the player does not interact with the game for 20 seconds, the program enters power-saving mode and turns off the LEDs and display segments.

This mode switches off the LEDs and display in software but does not put the Arduino itself into a hardware sleep state.

## Getting Started

1. Clone the repository to your computer.
2. Open the Arduino project in the Arduino IDE.
3. Make sure all required source files and header files are included.
4. Connect the LEDs, buttons, and display according to the pin configuration above.
5. Select the correct Arduino board and port.
6. Compile the program and upload it to the microcontroller.
7. Open the Serial Monitor at **9600 baud** to observe game events.

## Serial Monitor Messages

The program prints messages to the Serial Monitor to indicate the current game state.

- `PELI ALKAA!` – The game starts.
- `PELI PÄÄTTYI!` – The game ends after an incorrect button press.
- `PELI PÄÄTTYI ENNAKOINTIIN!` – The game ends because the player pressed a button too early.
- `VIRRANSÄÄSTÖTILA: PÄÄLLÄ` – Power-saving mode is activated.

These messages help monitor the game and troubleshoot potential issues.