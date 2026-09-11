#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"

// Use these 2 volatile variables for communicating between
// loop() function and interrupt handlers
volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile bool newTimerInterrupt = false;  // for timer interrupt handler
bool showingSequence = false;
int showIndex = 0;

byte score;
bool gameOver;
byte sequence[100];
byte sequenceLength;
byte playerIndex;

void setup() {
  Serial.begin(9600);
  for (byte i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  randomSeed(analogRead(A0));
}

unsigned long previousTime = 0;

void loop()
{
  if(gameOver) //uusi peli vanhan päättymisen jälkeen
  {
    if(digitalRead(buttonPins[1]) == LOW &&
        digitalRead(buttonPins[2]) == LOW)
    {
      startTheGame();
    }
  }

  if(!showingSequence && !gameOver) //käskyn odottaminen ensimmäisen pelin aloitukselle
{
    ledShow1();

    if(digitalRead(buttonPins[0]) == LOW &&
       digitalRead(buttonPins[3]) == LOW)
    {
        startTheGame();
    }
    else if(buttonNumber >= 0)
    {
        checkGame(buttonNumber);
        buttonNumber = -1;
    }
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
        setLed(sequence[showIndex]);
        delay(300);
        clearAllLeds();

        showIndex++;

        if(showIndex >= sequenceLength)
        {
            showingSequence = false;
            showIndex = 0;
            buttonNumber = -1; //tyhjentää vanhan painalluksen
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



void checkGame(byte button)
{
    if(button == sequence[playerIndex])
    {
        playerIndex++;

        if(playerIndex == sequenceLength)
        {
            score++;
            if(sequenceLength < 100)
            {
            sequenceLength++;
            }

            Serial.print("Pisteet: "); //print testaukseen
            Serial.println(score);

            playerIndex = 0;
            showingSequence = true;
            showIndex = 0;
        }
    }
    else
    {
        Serial.print("Peli päättyi. Pisteet: ");
        Serial.println(score);
        gameOver = true;
        showingSequence = false; //peliä ei voi jatkaa
        ledShow2();
        buttonNumber = -1;
    }
}

void initializeGame() //pelin lähtötilanne - lukemat nolla
{
    score = 0;
    sequenceLength = 1;
    playerIndex = 0;
    gameOver = false;

    for(byte i = 0; i < 100; i++) //pelin sekvenssin luominen
    {
        sequence[i] = random(0,4);
    }
}

void startTheGame()
{
  initializeGame();
  showingSequence = true;
  showIndex = 0;
}

