#include "buttons.h"

// Napeille taulukko.
uint8_t buttonPins[4] = {2, 3, 4, 5};

// Debouncea varten muuttuja.
volatile uint32_t viimeisinKeskeytys = 0;

// Tulostus aliohjelma. Tulostetaan serial monitorille mitä nappia on painettu. Tämä poistetaan lopullisesta Speden Spelistä.
void tulostaNapinPainallus(uint8_t nappi) {
  Serial.print("Nappia ");
  Serial.print(nappi);
  Serial.println(" painettu");
}

void initButtonsAndButtonInterrupts(void) {
  
  
  for (int i = firstPin; i <= lastPin; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  
  // Pin Change Interrupt Portille D.
  PCICR |= 0b00000100;
  
  // Valitaan pinnit D2-D5 käyttöön keskeytykselle.
  PCMSK2 |= 0b00111100;
}

// Keskeytyspalvelija pinneille D2-D5.
ISR(PCINT2_vect) {
  uint32_t nyt = millis();

// Ohjelmallinen debounce. Jos uusi keskeytys tulee alle 150 ms sisällä edellisestä, sitä ei huomioida.

  if (nyt - viimeisinKeskeytys < 300) {
    return;
  }
  viimeisinKeskeytys = nyt;

  if (digitalRead(buttonPins[0]) == LOW) {  // Jos nappia 2 painaa:
    tulostaNapinPainallus(1); // Kutsutaan tulostus aliohjelmaa.
    buttonNumber = 0;
  }

  else if (digitalRead(buttonPins[1]) == LOW) {
    tulostaNapinPainallus(2);
    buttonNumber = 1;
  }

  else if (digitalRead(buttonPins[2]) == LOW) {
    tulostaNapinPainallus(3);
    buttonNumber = 2;
  }

  else if (digitalRead(buttonPins[3]) == LOW) {
    tulostaNapinPainallus(4);
    buttonNumber = 3;
  }
}