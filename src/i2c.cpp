#include <i2c.h>
#include <globals.h>

I2C::I2C()
{
    Serial.println(F("Initializing I2C"));
    setup();
}

void I2C::setup()
{
    Wire.begin(); // Join The I2C Bus As A Master
    Serial.println(F("Wire initialized"));
}

// ----------------------------- Function that reads a register's data value --------------------------------------------------------------------
byte I2C::ReadRegister(int devaddr, byte regaddr)
{
    Wire.beginTransmission(devaddr);
    Wire.write(regaddr);
    Wire.endTransmission();
    Wire.requestFrom(devaddr, 1); // only one byte
    if (Wire.available())         // Wire.available indicates if data is available
        return Wire.read();       // Wire.read() reads the data on the wire
    else
        return 9999; // If no data in the wire, then return 9999 to indicate error
}

// ----------------------------- Function that writes a data value to a register --------------------------------------------------------------------
void I2C::WriteRegister(int devaddr, byte regaddr, byte dataval)
{
    Wire.beginTransmission(devaddr); // device
    Wire.write(regaddr);             // register
    Wire.write(dataval);             // data
    Wire.endTransmission();
}

// ----------------------------- Function that changes a single bit in a register --------------------------------------------------------------------
void I2C::ChangeBit(int devaddr, byte regaddr, int data, boolean setting)
{
    byte r = ReadRegister(devaddr, regaddr);
    if (setting == 1)
    {
        bitSet(r, data);
    }
    else if (setting == 0)
    {
        bitClear(r, data);
    }
    WriteRegister(devaddr, regaddr, r);
}