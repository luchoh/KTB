#include "extEEPROM.h" // Library for the I2C EEPROM

// I2C EEPROM stuff
extEEPROM myEEPROM(kbits_256, 1, 64, 0x50); // myEEPROM object, 256kbit, 1 device on the bus, 64 bytes page size, 0x50

class EEPROM
{

public:
    EEPROM();

    void setup();
};