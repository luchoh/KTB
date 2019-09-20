#include "eeprom.h"

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