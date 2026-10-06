#include "Arduino.h"
#include "display.h"

const int resetPin = 12;
const int shiftClockPin = 11;
const int latchClockPin = 10;
const int outEnablePin = 9;
const int serialInputPin = 8;

const uint8_t digits[] = {
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


void initializeDisplay(void)
{
  Serial.println("Initializing displays"); // Määritetään outputit
  pinMode(resetPin, OUTPUT);
  pinMode(shiftClockPin, OUTPUT);
  pinMode(latchClockPin, OUTPUT);
  pinMode(outEnablePin, OUTPUT);
  pinMode(serialInputPin, OUTPUT);

  digitalWrite(resetPin, HIGH);           // Määritetään vakiot
  digitalWrite(shiftClockPin, LOW);
  digitalWrite(latchClockPin, LOW);
  digitalWrite(outEnablePin, LOW);
  digitalWrite(serialInputPin, LOW);
// See requirements for this function from display.h
}


void writeByte(uint8_t bits,bool last)
{
  for(int i = 7; i >= 0; i--)
  {
    uint8_t bitValue = (bits >> i) & 0x01;
    digitalWrite(serialInputPin, bitValue);

    digitalWrite(shiftClockPin, HIGH);
    digitalWrite(shiftClockPin, LOW);
  }

  if (last)
  {
    digitalWrite(latchClockPin, HIGH);
    digitalWrite(latchClockPin, LOW);
  }
// See requirements for this function from display.h
}


void writeHighAndLowNumber(uint8_t tens,uint8_t ones)
{
  writeByte(digits[tens], false);
  writeByte(digits[ones], true);
// See requirements for this function from display.h
}


void showResult(byte number)
{
  uint8_t tens = number / 10;
  uint8_t ones = number % 10;

  writeHighAndLowNumber(tens, ones);
// See requirements for this function from display.h
}

void clearDisplay(void)
{
  writeByte(0x00, false);
  writeByte(0x00, true);
}