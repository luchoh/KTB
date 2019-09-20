#include "rotary.h"
#include <Arduino.h>

ROTARY::ROTARY()
{
}

void ROTARY::setup()
{
    // Set up the pin modes
    pinMode(VOLUPPIN, INPUT);     // Button switch or Encoder pin for volume up
    digitalWrite(VOLUPPIN, HIGH); // If H/W debouncing is implemented, set to LOW

    pinMode(VOLDOWNPIN, INPUT);     // Button switch or Encoder pin for volume down
    digitalWrite(VOLDOWNPIN, HIGH); // If H/W debouncing is implemented, set to LOW

    pinMode(SELECTPIN, INPUT_PULLUP); // Button switch or Encoder pin for Select

    pinMode(POWERPIN, OUTPUT);    // Power relay control pin
    digitalWrite(POWERPIN, HIGH); // Keep high on powerup

    pinMode(INTERRUPTPIN, INPUT); // Interrupt pin

    pinMode(SIDECARPIN, OUTPUT);   // SSS control pin
    digitalWrite(SIDECARPIN, LOW); // By default select I2S input
}
