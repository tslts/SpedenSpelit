#include "buttons.h"




void initButtonsAndButtonInterrupts(void)
{
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    pinMode(6, INPUT_PULLUP);
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