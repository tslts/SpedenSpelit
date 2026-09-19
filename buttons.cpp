#include "buttons.h"


extern const byte buttonPins[4] = { 2, 3, 4, 5 };

void initButtonsAndButtonInterrupts(void)
{
    pinMode(buttonPins[0], INPUT_PULLUP);
    pinMode(buttonPins[1], INPUT_PULLUP);
    pinMode(buttonPins[2], INPUT_PULLUP);
    pinMode(buttonPins[3], INPUT_PULLUP);
    // keskeytysten asetukset
}


ISR(PCINT2_vect)
{
    if(digitalRead(2) == LOW)
        buttonNumber = 0;

    if(digitalRead(3) == LOW)
        buttonNumber = 1;

    if(digitalRead(4) == LOW)
        buttonNumber = 2;

    if(digitalRead(5) == LOW)
        buttonNumber = 3;
}