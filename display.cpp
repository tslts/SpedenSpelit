#include "Arduino.h"
#include "display.h"

const int resetPin = 12;
const int shiftClockPin = 11;
const int latchClockPin = 10;
const int outEnablePin = 9;
const int serialInputPin = 8;

const uint8_t digits[] = {  // Lukujen segmenttikoodit
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};


void initializeDisplay(void) {

  pinMode(resetPin, OUTPUT);  // Output määritys
  pinMode(shiftClockPin, OUTPUT);
  pinMode(latchClockPin, OUTPUT);
  pinMode(outEnablePin, OUTPUT);
  pinMode(serialInputPin, OUTPUT);

  digitalWrite(resetPin, HIGH); // Vakioiden määritys
  digitalWrite(shiftClockPin, LOW);
  digitalWrite(latchClockPin, LOW);
  digitalWrite(outEnablePin, LOW);
  digitalWrite(serialInputPin, LOW);
}

void writeByte(uint8_t bits, bool last) {

  for(int i = 7; i >= 0; i--) { 
    uint8_t bitValue = (bits >> i) & 1; // Poimitaan bitti kerrallaan
    digitalWrite(serialInputPin, bitValue); // Bitti dataväylään
    digitalWrite(shiftClockPin, HIGH);  // Kellopulssi: Bitti rekisteriin
    digitalWrite(shiftClockPin, LOW);
  }

  if(last) {  // Kaikki tavut lähetetty: latch-pulssi
    digitalWrite(latchClockPin, HIGH);
    digitalWrite(latchClockPin, LOW);
  }
}

void writeHighAndLowNumber(uint8_t tens, uint8_t ones) {

  writeByte(digits[tens], false); // Kymmenet
  writeByte(digits[ones], true);  // Ykköset (latch)
}

void showResult(byte number) {

  uint8_t tens = number / 10; // Kymmenet
  uint8_t ones = number % 10; // Ykköset
  writeHighAndLowNumber(tens, ones);
}

void clearDisplay(void) { // Virransäästötila: Segmenttien sammutus

  writeByte(0, false);
  writeByte(0, true);
}