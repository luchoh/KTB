#ifndef _I2C_H
#define _I2C_H

#include <Wire.h>              // Library for the I2C communications

class I2C
{

public:
    I2C();

    void setup();

    byte ReadRegister(int devaddr, byte regaddr);
    void WriteRegister(int devaddr, byte regaddr, byte dataval);
    void ChangeBit(int devaddr, byte regaddr, int data, boolean setting);
};

#endif