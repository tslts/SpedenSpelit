#include "leds.h"

const byte ledPins[] = {A2, A3, A4, A5};
// Show1 Muuttuja.
uint8_t laskuri = 0;

// Show2 Muuttujat.
uint8_t vaihe = 0;
uint8_t toisto = 0;

uint16_t viive = 500;
uint32_t viimeisinAika = 0;

void initializeLeds() {

  // Analog ledPins output tilaan.
  for (uint8_t i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void setLed(byte ledNumber)
{
    clearAllLeds();

    if(ledNumber < 4)
    {
        digitalWrite(ledPins[ledNumber], LOW);
    }
}


void clearAllLeds() {
  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
}

void setAllLeds() {
  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}


void ledShow1() {

  laskuri++; 
  delay(200); 

  if((laskuri % 4) > 1) { digitalWrite(A2, LOW); } else { digitalWrite(A2, HIGH); }

  if((laskuri % 8) > 3) { digitalWrite(A3, LOW); } else { digitalWrite(A3, HIGH); }

  if((laskuri % 16) > 7) { digitalWrite(A4, LOW); } else { digitalWrite(A4, HIGH); }

  if((laskuri % 32) > 15) { digitalWrite(A5, LOW); } else { digitalWrite(A5, HIGH); } 

  delay(200); 

}

// Viive säädetty kolmelle toistolle.
void ledShow2(byte rounds) {

  uint32_t nyt = millis();

  // Aloitetaan loop, jos on kulunut vähintään 500 ms.
  if (nyt - viimeisinAika >= viive) {
    
    viimeisinAika = nyt;

    if (vaihe < 4) {

      // Sytytetään yksi LED kerrallaan.
      digitalWrite(ledPins[vaihe], LOW);
      vaihe++;

    } else {

      // Kun kaikki LEDit palaa, sammutetaan ne.
      for (uint8_t i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], HIGH);
      }
    
      // Vähennetään viivettä.
      viive -= 150;
      vaihe = 0;
      toisto++;

      if (toisto >= rounds) {
        toisto = 0;
        viive = 500;
      }
    }
  }
}