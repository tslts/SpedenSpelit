#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"

// Use these 2 volatile variables for communicating between
// loop() function and interrupt handlers
volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile bool newTimerInterrupt = false;  // for timer interrupt handler
bool showingSequence = false;

bool sleep = false;
unsigned long idleTime = 0;
bool gameOver;
byte sequence[100];
int gameIndex = 0;
int score = 0;
byte playerIndex;


void setup() {
  Serial.begin(9600);
  initializeLeds();
  initializeDisplay();
  initButtonsAndButtonInterrupts();
  initializeTimer();
  idleTime = millis();

  randomSeed(analogRead(A0));
}


void loop()
{
  if(gameOver)
  {
    if(digitalRead(buttonPins[1]) == LOW &&
        digitalRead(buttonPins[2]) == LOW){
            startGame();
            idleTime = millis();
    }
  }

  if(!showingSequence && !gameOver) //lähtötilanne, odotetaan pelaajan painallusta
{
    ledShow1();

    if(digitalRead(buttonPins[0]) == LOW &&
       digitalRead(buttonPins[3]) == LOW)
    {
        startGame();
        idleTime = millis();
    }
}
if(buttonNumber >= 0 && showingSequence && !gameOver) //pelaajan painalluksia tarkistetaan vain sekvenssin pyöriessä
    {
        idleTime = millis();
        checkGame(buttonNumber);
        buttonNumber = -1; //tyhjennetään painikemuuttuja seuraavalle painallukselle
    }


if(newTimerInterrupt)
{
    newTimerInterrupt = false;

    if(showingSequence)
    {
        setLed(sequence[gameIndex]);

        gameIndex++;
        if(gameIndex > 0 && gameIndex % 10 == 0){
        OCR1A = (OCR1A * 9) / 10; //kokonaisluku tarkempi kuin double mikrokontrollerilla
    }

        if(gameIndex >= 100)
        {
            playerIndex = 0;
            gameIndex = 0;            
        }
    }
}
// virransäästö, 20 sekuntia ilman inputtia sammuttaa ledit
if(!sleep && millis() - idleTime >= 20000){
    sleep = true;
    clearAllLeds();
}
}


void initializeTimer(void)
{
    cli(); // keskeytykset pois
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    // 16 MHz / 1024 = 15625 Hz
    OCR1A = 15624; // 1 sekunti
    TCCR1B |= (1 << WGM12); // CTC mode
    TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler 1024
    TIMSK1 |= (1 << OCIE1A); // compare match A interrupt
    sei(); // keskeytykset päälle
}
ISR(TIMER1_COMPA_vect)
{
    newTimerInterrupt = true;
}

void createSequence() // luo satunnaisen sekvenssin
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
}

void initializeGame()
{
    createSequence();
    playerIndex = 0;
    gameIndex = 0;
    gameOver = false;
    score = 0;
    buttonNumber = -1;
    newTimerInterrupt = false;
    OCR1A = 15624; // 1 sekunti

}

void checkGame(byte button)
{
    if(playerIndex >= gameIndex){
        gameOver = true;
        ledShow2();
        showingSequence = false; //sekvenssin näyttäminen pysähtyy, peliä ei voi jatkaa
        return; // pelaaja yritti arvata, peli pääättyy
    }
    if(button == sequence[playerIndex]){
        playerIndex++;
        score++;
        showResult(score);
        Serial.print("Pisteet: "); //print testaukseen
        Serial.println(score);
    }else{
        Serial.print("Peli päättyi. Pisteet: ");
        Serial.println(score);
        gameOver = true;
        showingSequence = false; //sekvenssin näyttäminen pysähtyy, peliä ei voi jatkaa
        ledShow2();
        buttonNumber = -1;
    }
}

