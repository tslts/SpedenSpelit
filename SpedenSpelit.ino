#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"

volatile int buttonNumber = -1;
volatile bool newTimerInterrupt = false;

bool showingSequence = false;
bool waitStart = false;
bool gameOver = false;
bool ledOnPhase = false;
bool sleep = false;

unsigned long countdownStart = 0;
unsigned long idleTime = 0;

byte sequence[200];
byte playerIndex = 0;

int gameIndex = 0;
int score = 0;

void setup() {

  Serial.begin(9600);
  initializeLeds();
  initializeDisplay();
  clearDisplay();
  initButtonsAndButtonInterrupts();
  initializeTimer();
  idleTime = millis();
  randomSeed(analogRead(A0));
}
 
void loop() {

  if(gameOver) {

    if(buttonNumber == 1) {
      Serial.println("PELI ALKAA!");
      startGame();
      buttonNumber = -1;
      idleTime = millis();
    }
  }

  if(waitStart) {

    unsigned long timePassed = millis() - countdownStart;

    if(timePassed < 1000) { // Näytöllä 3 sekunnin countdown ennen pelin alkua
      showResult(3);
    }

    else if(timePassed < 2000) {
      showResult(2);
    }
    
    else if(timePassed < 3000) {
      showResult(1);
    }

    else if(timePassed < 4000) {
      showResult(0);

    } else {

      waitStart = false;
      showingSequence = true;
      gameIndex = 0;
      ledOnPhase = true;
      buttonNumber = -1;
      setLed(sequence[gameIndex]);

      TCNT1 = 0;
      TIFR1 |= (1 << OCF1A);
      newTimerInterrupt = false; 
    }
  }

  if(!showingSequence && !gameOver && !waitStart) { // Lähtötilanne, odotetaan pelaajan painallusta

    if(!sleep) {
      ledShow1();
    }

    if(buttonNumber == 0) {
      Serial.println("PELI ALKAA!");
      sleep = false;
      startGame();
      buttonNumber = -1;
      idleTime = millis();
    }
  }

  if(buttonNumber >= 0 && showingSequence && !gameOver) { // Pelaajan painalluksia tarkistetaan vain sekvenssin pyöriessä
    idleTime = millis();
    checkGame(buttonNumber);
    buttonNumber = -1;  // Tyhjennetään painikemuuttuja seuraavalle painallukselle
  }

  if(newTimerInterrupt) {
    newTimerInterrupt = false;

    if(showingSequence && !waitStart) {
     
      if(!ledOnPhase) {
        setLed(sequence[gameIndex]);  // Sytytetään seuraava LED
        ledOnPhase = true;

      } else {
        clearAllLeds(); // Sammutetaan LED välivaiheeksi
        gameIndex++;

        if(gameIndex > 0 && gameIndex % 10 == 0) {
          OCR1A = OCR1A * 0.9;
          TCNT1 = 0;
        }

        if(gameIndex >= 200) {
          showingSequence = false;
          gameOver = true;
          setLed(1);
          idleTime = millis();
        }

        ledOnPhase = false;
      }
    }
  }

  if(!sleep && !showingSequence && !waitStart && (millis() - idleTime >= 20000)) {  // Virransäästötila: 20 sekuntia ilman inputtia sammuttaa ledit ja näytöt
    sleep = true;
    Serial.println("VIRRANSÄÄSTÖTILA: PÄÄLLÄ");
    clearAllLeds();
    clearDisplay();
  }
}
 
void initializeTimer(void) {
  cli();  // Keskeytykset pois
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
 
  // 16 MHz / 1024 = 15625 Hz
  OCR1A = 15624; // 1 sekunti
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10);  // Prescaler 1024
  TIMSK1 |= (1 << OCIE1A); // compare match A interrupt
  sei();  // Keskeytykset päälle
}

ISR(TIMER1_COMPA_vect) {

  newTimerInterrupt = true;
}

void createSequence() { // Luo satunnaisen sekvenssin

  for(byte i = 0; i < 200; i++) {
    sequence[i] = random(0,4);
  }
}

void startGame() {

  initializeGame();
  clearAllLeds();
  waitStart = true;
  showingSequence = false;
  countdownStart = millis();
}

void initializeGame() {

  createSequence();
  playerIndex = 0;
  gameIndex = 0;
  ledOnPhase = false;
  gameOver = false;
  score = 0;
  TCNT1 = 0;  // Aloittaa timer1 alusta
  OCR1A = 15624;
  newTimerInterrupt = false;
  sleep = false;
}
 
void checkGame(byte button) {

  if(playerIndex > gameIndex || (playerIndex == gameIndex && !ledOnPhase)) {
    Serial.print("PELI PÄÄTTYI ENNAKOINTIIN! Pisteet: ");
    Serial.println(score);
    gameOver = true;
    showingSequence = false;  // Sekvenssin näyttäminen pysähtyy, peliä ei voi jatkaa
    waitStart = false;
    ledShow2(1);
    delay(1000);
    setLed(1);
    buttonNumber = -1;
    idleTime = millis();

    return; // Pelaaja yritti arvata, peli päättyy
  }

  if(button == sequence[playerIndex]) {
    playerIndex++;
    score++;
    showResult(score % 100);

  } else {
    Serial.print("PELI PÄÄTTYI! Pisteet: ");
    Serial.println(score);
    gameOver = true;
    showingSequence = false;  // Sekvenssin näyttäminen pysähtyy, peliä ei voi jatkaa
    waitStart = false;
    ledShow2(1);
    delay(1000);
    setLed(1);
    buttonNumber = -1;
    idleTime = millis();
  }
}