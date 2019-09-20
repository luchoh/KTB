#include "eeprom.h"


int eeAddress = 0;     // EEPROM address
int eeAddress1 = 0;    // EEPROM address of Input 1 memory
int eeAddress2 = 128;  // EEPROM address of Input 2 memory
int eeAddress3 = 256;  // EEPROM address of Input 3 memory
int eeAddress4 = 384;  // EEPROM address of Input 4 memory
int eeAddress5 = 512;  // EEPROM address of Input 5 memory
int eeAddress6 = 640;  // EEPROM address of Input 6 memory
int eeAddressS = 1920; // EEPROM address of Settings memory
int eeAddressT = 2048; // EEPROM address of test data

EEPROM::EEPROM()
{
}

void EEPROM::setup()
{
    // Start communication with the I2C EEPROM chip
    byte i2cStat = myEEPROM.begin(myEEPROM.twiClock100kHz);
    if (i2cStat != 0)
    {
        Serial.println(F("Problem communicating with EEPROM chip."));
    }
}