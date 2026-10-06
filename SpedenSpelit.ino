#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"
 
// Use these 2 volatile variables for communicating between
// loop() function and interrupt handlers
volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile bool newTimerInterrupt = false;  // for timer interrupt handler
bool showingSequence = false;
 
bool waitStart = false;
unsigned long countdownStart = 0;
const unsigned long startTime = 2000;
 
bool sleep = false;
unsigned long idleTime = 0;
bool gameOver = false;
byte sequence[100];
int gameIndex = 0;
int score = 0;
byte playerIndex;
bool menuAnimationEnabled = true; //tarvitaanko enää?
bool ledOnPhase = false;
 
 
void setup() {
  Serial.begin(9600);
  initializeLeds();
  initializeDisplay();
  initButtonsAndButtonInterrupts();
  initializeTimer();
  showResult(0);
  idleTime = millis();
 
  randomSeed(analogRead(A0));
}
 
 
void loop()
{
  if(gameOver)
  {
    
    /*if(digitalRead(buttonPins[1]) == LOW  &&
        digitalRead(buttonPins[2]) == LOW)*/
      if(buttonNumber == 1)  
        {
            Serial.println("Peli alkoi");
            startGame();
            buttonNumber = -1;
            idleTime = millis();
    }
  }
if(waitStart){
    
    if(millis() - countdownStart >= startTime){
        waitStart = false;
        buttonNumber = -1;
        showingSequence = true;
        gameIndex = 0;
        ledOnPhase = false;
        
        clearAllLeds();
        setLed(sequence[gameIndex]);
        //gameIndex++;
        TCNT1 = 0;
        TIFR1 |= (1<< OCF1A);
        newTimerInterrupt = true;

       
    }
}
  if(!showingSequence && !gameOver && !waitStart) //lähtötilanne, odotetaan pelaajan painallusta
{
    if(!sleep)
    {
    if(menuAnimationEnabled)
    {
    ledShow1();
    }
    }
    /*if(digitalRead(buttonPins[0]) == LOW  &&
       digitalRead(buttonPins[3]) == LOW)*/
       if (buttonNumber == 0)
    {
        Serial.println("Peli alkoi");
        sleep = false;
        startGame();
        buttonNumber = -1;
        idleTime = millis();
    }
}
if(buttonNumber >= 0 && showingSequence && !gameOver) //pelaajan painalluksia tarkistetaan vain sekvenssin pyöriessä
    {
        idleTime = millis();
        checkGame(buttonNumber);
        buttonNumber = -1; //tyhjennetään painikemuuttuja seuraavalle painallukselle
    }
 
 
/*if(newTimerInterrupt)
{
    newTimerInterrupt = false;
 
    if(showingSequence && !waitStart)
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
}*/

if(newTimerInterrupt)
{
newTimerInterrupt = false;

if(showingSequence && !waitStart)
{
if(!ledOnPhase)
{
// Sytytetään seuraava LED
setLed(sequence[gameIndex]);

ledOnPhase = true;
}
else
{
// Sammutetaan LED välivaiheeksi
clearAllLeds();

gameIndex++;

if(gameIndex > 0 && gameIndex % 10 == 0)
{
OCR1A = OCR1A * 0.5;
}

if(gameIndex >= 100)
{
playerIndex = 0;
gameIndex = 0;
}

ledOnPhase = false;
}
}
}

// virransäästö, 20 sekuntia ilman inputtia sammuttaa ledit
if(!sleep && (millis() - idleTime >= 20000)){
    sleep = true;
    clearAllLeds();
    clearDisplay();
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
    menuAnimationEnabled = false;
  initializeGame();
  clearAllLeds();
  waitStart = true;
  showingSequence = false;
  countdownStart = millis();
 
  TCNT1 = 0; // aloittaa timer1 alusta
  OCR1A = 15624;
  newTimerInterrupt = false;
}
 
void initializeGame()
{
    createSequence();
    playerIndex = 0;
    gameIndex = 0;
    ledOnPhase = false;
    gameOver = false;
    score = 0;
    buttonNumber = -1;
    newTimerInterrupt = false;
    OCR1A = 15624;
    sleep = false;
    showResult(0);
}
 
void checkGame(byte button)
{
    if(playerIndex > gameIndex){
        Serial.print("Peli päättyi ennakointiin. Pisteet: ");
        Serial.println(score);
        gameOver = true;
        ledShow2(1);
        showingSequence = false; //sekvenssin näyttäminen pysähtyy, peliä ei voi jatkaa
        waitStart = false;
        buttonNumber = -1;
        clearAllLeds();
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
        waitStart = false;
        ledShow2(1);
        buttonNumber = -1;
        clearAllLeds();
    }
}
