Spede's Games – Arduino Game
Project Description

This project implements a reaction and memory game inspired by Spede's Games using Arduino. The objective is to observe a sequence of LED lights and reproduce it by pressing the correct buttons in the correct order.

As the game progresses, the sequence becomes faster, requiring increasingly better memory and reaction speed. The game ends if the player presses the wrong button, presses a button too early, or reaches the end of the sequence.

The project provides hands-on experience with microcontroller programming, interrupts, timers, digital output control, and modular software design.

- Features
- Four LEDs for displaying the sequence.
- Four buttons for player input.
- Two-digit seven-segment display for showing the score.
- Randomly generated LED sequences.
- Timer-based sequence control.
- Increasing game speed as the player progresses.
- Software-based button debouncing.
- Power-saving mode activated after 20 seconds of inactivity.
- Serial output for monitoring game events.
- Hardware Components
Component	Purpose
Arduino-compatible microcontroller	Controls the game
4 LEDs	Display the game sequence
4 push buttons	Player input
Seven-segment display	Displays the score and countdown
Shift registers	Control the display segments
Resistors and jumper wires	Electrical connections

Note: The display is controlled using shift registers. The exact wiring depends on the hardware being used.

Pin Configuration

LED	Arduino Pin
LED 0	      A2
LED 1	      A3
LED 2	      A4
LED 3	      A5

The LEDs use active-LOW logic: LOW turns an LED on, while HIGH turns it off.

Buttons
Button	Arduino Pin
Button  0	      D2
Button  1	      D3
Button  2	      D4
Button  3	      D5

The buttons use the INPUT_PULLUP configuration. Pressing a button connects the pin to ground, causing its input value to become LOW.

Button state changes are detected using a pin change interrupt on Port D.

Seven-Segment Display
Signal	      Arduino Pin
Reset	        D12
Shift Clock	  D11
Latch Clock	  D10
Output Enable	D9
Serial Input	D8

The display is controlled by shifting segment bit patterns into shift registers. The program supports digits from 0 to 9 and displays values using two digit positions.

How the Game Works
Idle state: The LEDs perform an introductory light show.
Starting the game: The player starts a new game by pressing the designated start button.
Countdown: The display counts down from 3 to 0.
Sequence display: The LEDs light up in a randomly generated sequence.
Player's turn: The player reproduces the sequence by pressing the corresponding buttons in the correct order.
Scoring: Each correct button press increases the score.
Increasing difficulty: The sequence playback becomes faster as the game progresses.
Game over: An incorrect button press or a premature guess ends the game.
Restart: The player can start a new round.


Software Architecture

The program is divided into modules to separate hardware control from the main game logic.

File	Responsibility
SpedenSpelit.ino	Main program, game states, scoring, and timer handling
leds.h / leds.cpp	LED control and light show routines
buttons.h / buttons.cpp	Button initialization and interrupts
display.h / display.cpp	Seven-segment display control

These filenames describe the intended module responsibilities. If your project uses different filenames or combines implementations into fewer files, update this section to match the actual repository structure.

Key Functions
initializeLeds() – Initializes the LED pins.
setLed() – Turns on the selected LED and turns off the others.
clearAllLeds() – Turns off all LEDs.
ledShow1() – Runs the idle-state light show.
ledShow2() – Runs an accelerating light show.
initButtonsAndButtonInterrupts() – Initializes the buttons and interrupts.
initializeDisplay() – Initializes the display control.
showResult() – Displays the score.
initializeTimer() – Configures the Timer1 hardware timer.
createSequence() – Generates a random sequence.
startGame() – Starts a new round.
checkGame() – Checks whether the player's button press is correct.


Technical Implementation

Timer and Interrupts
The LED sequence is controlled using the Timer1 hardware timer, suitable for ATmega-based Arduino boards. The timer operates in CTC mode with a prescaler of 1024.
At a clock frequency of 16 MHz, the initial comparison value for a one-second interval is set to OCR1A = 15624.
The timer interrupt service routine sets the newTimerInterrupt flag. The main game logic then processes this flag inside the loop() function.

Button Debouncing
Mechanical button bounce is filtered using a software-based approach. A 300-millisecond interval is applied between accepted interrupts, and additional interrupts within that interval are ignored.

Power-Saving Mode
If the player does not interact with the game for 20 seconds, the program enters power-saving mode and turns off the LEDs and display segments.
This mode switches off the LEDs and display in software but does not put the Arduino itself into a hardware sleep state.


The program prints messages to the Serial Monitor to indicate the current game state, for example:
PELI ALKAA! – The game starts.
PELI PÄÄTTYI! – The game ends after an incorrect button press.
PELI PÄÄTTYI ENNAKOINTIIN! – The game ends because the player pressed a button too early.
VIRRANSÄÄSTÖTILA: PÄÄLLÄ – Power-saving mode is activated.

These messages help monitor the game and troubleshoot potential issues.
