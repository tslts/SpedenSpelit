#include "leds.h"

void initializeLeds()
{
    for(byte i = 0; i < 4; i++)
    {
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], LOW);
    }
}

void setLed(byte ledNumber)
{
    clearAllLeds();

    if(ledNumber < 4)
    {
        digitalWrite(ledPins[ledNumber], HIGH);
    }
}

void clearAllLeds()
{
    for(byte i = 0; i < 4; i++)
    {
        digitalWrite(ledPins[i], LOW);
    }
}

void setAllLeds()
{
    for(byte i = 0; i < 4; i++)
    {
        digitalWrite(ledPins[i], HIGH);
    }
}

void ledShow1()
{
    byte pattern[] = {
        0, 1, 2, 3,
        2, 1,
        0, 1, 2, 3
    };

    for(byte i = 0; i < sizeof(pattern); i++)
    {
        digitalWrite(ledPins[pattern[i]], HIGH);
        delay(100);

        digitalWrite(ledPins[pattern[i]], LOW);
        delay(50);
    }
}

void ledShow2()
{
    for(byte i = 0; i < 5; i++)
    {
        setAllLeds();
        delay(100);

        clearAllLeds();
        delay(100);
    }
}