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

uint32_t nytLed1 = millis();


if(nytLed1 - viimeisinAika > 200)
{
viimeisinAika = nytLed1;
laskuri++;
}

  if((laskuri % 4) > 1) { digitalWrite(A2, LOW); } else { digitalWrite(A2, HIGH); }

  if((laskuri % 8) > 3) { digitalWrite(A3, LOW); } else { digitalWrite(A3, HIGH); }

  if((laskuri % 16) > 7) { digitalWrite(A4, LOW); } else { digitalWrite(A4, HIGH); }

  if((laskuri % 32) > 15) { digitalWrite(A5, LOW); } else { digitalWrite(A5, HIGH); } 


}

void ledShow2(byte rounds)
{
int viive = 500;

for(byte r = 0; r < rounds; r++)
{
// Nopeutuva valoshow
while(viive >= 100)
{
// LED0
clearAllLeds();
delay(100);
digitalWrite(ledPins[0], LOW);
delay(viive);

// LED0 + LED1
clearAllLeds();
digitalWrite(ledPins[0], LOW);
digitalWrite(ledPins[1], LOW);
delay(viive);

// LED0 + LED1 + LED2
clearAllLeds();
digitalWrite(ledPins[0], LOW);
digitalWrite(ledPins[1], LOW);
digitalWrite(ledPins[2], LOW);
delay(viive);

// LED0 + LED1 + LED2 + LED3
clearAllLeds();
digitalWrite(ledPins[0], LOW);
digitalWrite(ledPins[1], LOW);
digitalWrite(ledPins[2], LOW);
digitalWrite(ledPins[3], LOW);
delay(viive);

viive -= 50;
}

viive = 500;
}
clearAllLeds();
}