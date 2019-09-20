#include <Arduino.h>

volatile int dir;            // Rotary encoder variable.
volatile int rotPosition;    // Rotary encoder variable.
volatile int oldRotPosition; // Rotary encoder variable.

int VOLUPPIN = 6;   // RotEnc A terminal for right rotary encoder.
int VOLDOWNPIN = 7; // RotEnc B terminal for right rotary encoder.
#define SELECTPIN 5 // Rotary Encoder button pin.

int VOLUPPIN2 = A4;   // RotEnc A terminal for left rotary encoder.
int VOLDOWNPIN2 = A3; // RotEnc B terminal for left rotary encoder.
#define SELECTPIN2 A5 // Switch to select function for left rotary encoder.

int INTERRUPTPIN = 3; // Connect to Interrupt pin on DAC (GPIO4).
int SIDECARPIN = A1;  // Connect to Solid State Sidecar control pin (not to be used with the original TPA Sidecar!!! Danger of damage!!).
int POWERPIN = A0;    // Power relay pin.

class ROTARY
{

public:
    ROTARY();

    void setup();
};