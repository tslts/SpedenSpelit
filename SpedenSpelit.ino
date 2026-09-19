#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"

// Use these 2 volatile variables for communicating between
// loop() function and interrupt handlers
volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile bool newTimerInterrupt = false;  // for timer interrupt handler
bool showingSequence = false;


bool gameOver;
byte sequence[100];
int gameIndex = 0;
int score = 0;
byte playerIndex;

void setup() {
  Serial.begin(9600);
  initializeLeds();
  initButtonsAndButtonInterrupts();
  randomSeed(analogRead(A0));
}

unsigned long previousTime = 0;

void loop()
{
  if(gameOver) //lähtötilanne vanhan pelin jälkeen, odotetaan pelaajan painallusta
  {
    ledShow2();
    if(digitalRead(buttonPins[0]) == LOW &&
       digitalRead(buttonPins[3]) == LOW)
    {
        startGame();
    }
  }
  {
    if(digitalRead(buttonPins[1]) == LOW &&
        digitalRead(buttonPins[2]) == LOW)
    {
      startGame();
    }
  }

  if(!showingSequence && !gameOver) //lähtötilanne, odotetaan pelaajan painallusta
{
    ledShow1();

    if(digitalRead(buttonPins[0]) == LOW &&
       digitalRead(buttonPins[3]) == LOW)
    {
        startGame();
    }
}
if(buttonNumber >= 0 && showingSequence && !gameOver) //pelaajan painalluksia tarkistetaan vain sekvenssin pyöriessä
    {
        checkGame(buttonNumber);
        buttonNumber = -1; //tyhjennetään painikemuuttuja seuraavalle painallukselle
    }

    // Timerin simulointi R4:llä
    if(millis() - previousTime >= 1000)
    {
        previousTime = millis();
        newTimerInterrupt = true;
    }

    // Tämä vastaa myöhemmin TIMER1-keskeytystä
if(newTimerInterrupt)
{
    newTimerInterrupt = false;

    if(showingSequence)
    {
        setLed(sequence[gameIndex]);
        delay(300);
        clearAllLeds();

        gameIndex++;
        if(gameIndex >= 100)
        {
            playerIndex = 0;
            gameIndex = 0;            
        }
    }
}
}


void initializeTimer(void)
{
	// see requirements for the function from SpedenSpelit.h
}
ISR(TIMER1_COMPA_vect)
{
  /*
  Communicate to loop() that it's time to make new random number.
  Increase timer interrupt rate after 10 interrupts.
  */
  
}

void createSequence() //luo satunnaisen sekvenssin
{
    for(byte i = 0; i < 100; i++)
    {
        sequence[i] = random(0,4);
    }
}

void startGame()
{
  initializeGame();
  showingSequence = true;
  gameIndex = 0;
}

void initializeGame()
{
    createSequence();
    playerIndex = 0;
    gameIndex = 0;
    gameOver = false;
    score = 0;
    buttonNumber = -1;
}

void checkGame(byte button)
{
    if(playerIndex >= gameIndex){
        gameOver = true;
        ledShow2();
        showingSequence = false; //sekvenssin näyttäminen pysähtyy, peliä ei voi jatkaa
        return; // pelaaja yritti arvata, peli pääättyy
    }
    if(button == sequence[playerIndex])
    {
        playerIndex++;
        score++;
        Serial.print("Pisteet: "); //print testaukseen
        Serial.println(score);
    }
    else
    {
        Serial.print("Peli päättyi. Pisteet: ");
        Serial.println(score);
        gameOver = true;
        showingSequence = false; //sekvenssin näyttäminen pysähtyy, peliä ei voi jatkaa
        ledShow2();
        buttonNumber = -1;
    }
}

