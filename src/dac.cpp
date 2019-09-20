#include <dac.h>

byte dacAddr = 0x48; // address of DAC chip
int dac_chip = 0;    // Type of DAC chip for auto detection. 0 = ES9038Pro, 1 = ES9028Pro

#ifdef ALWAYSON
bool poweron = true; // Variable to hold whether the DAC is in power-on state or not
#else
bool poweron = false; // Variable to hold whether the DAC is in power-on state or not
#endif

DAC_Input Input[TOTALINPUTS]; // Declare structures of Inputs

int IIR = 0;
int mute = 0;
int DPLL_PCM = 5;
int DPLL_DSD = 5;
bool Deemphasis_en = 0;
bool Deemphasis_auto = 0;
int Deemphasis_sel = 0;
int LockSpeed = 0;
bool Dither = 1;
bool JitterElim = 1;
bool bypass_osf = 0;
bool deemph_bypass = 0;
bool auto_deemph = 0;
int deemph_sel = 0;
int SR = 0;
int SR_Old = 0;
int sig_type = 0;
int sig_typeOld = 0;
bool lock = 0;
bool lockOld = 0;

I2C *i2c;

DAC::DAC()
{
    Serial.println(F("Initializing DAC"));
    i2c = new I2C();
    if (poweron == true)
    {
        DAC::setup(); // Initialize the DAC chip
        // setInput();   // Set the input to the default input
        //main_disp(true);
    }
}

void DAC::setup()
{
    Serial.print(F("Setting up DAC"));
    i2c->ChangeBit(dacAddr, 0x00, 0, 1); // Put DAC in RESET mode
    delay(500);                          // Wait for 500ms
    i2c->ChangeBit(dacAddr, 0x00, 0, 0); // Take DAC out of RESET mode
    mute = 1;                            // Put DAC in mute
    // setMute();                  // Put DAC in mute
    i2c->ChangeBit(dacAddr, 0x09, 7, 0); // Setup GPIO4
    i2c->ChangeBit(dacAddr, 0x09, 6, 1); // Setup GPIO4
    i2c->ChangeBit(dacAddr, 0x09, 5, 0); // Setup GPIO4
    i2c->ChangeBit(dacAddr, 0x09, 4, 0); // Setup GPIO4
    i2c->ChangeBit(dacAddr, 0x0F, 2, 1); // Set DAC up for Stereo Mode
    i2c->ChangeBit(dacAddr, 0x36, 7, 0); // Enable DoP support
    // mute = 0;                   // Take DAC out of mute
    // setMute();                  // Take DAC out of mute
    // preamp = Settings1.variable;
    // preamp_def_vol = Settings1.preamp_def_vol;

    // if (preamp == true)
    // {
    //     volume = preamp_def_vol;
    // }

    // input = 0;
    // setInput();
    // setVol();
}

byte DAC::getDacAddress()
{
    return dacAddr;
}

// ----------------------------- Function that prints the values of registers for debugging --------------------------------------------------------------------
void DAC::PrintDebugRegAll(byte dac)
{
    Serial.println("");

    Serial.print(F("ALL Register values for DAC: "));
    Serial.println("0x" + String(dac, HEX));

    byte r = 0;
    for (int i = 0; i < 100; i++)
    {
        Serial.print("Register 0x" + String(i, HEX) + ":");
        r = i2c->ReadRegister(dac, i);
        Serial.println(r, BIN);
        r = 0;
    }
}

// ----------------------------- Function that prints the values of registers for debugging --------------------------------------------------------------------
void DAC::PrintDebugReg(byte dac)
{
    Serial.println("");

    Serial.print(F("Register values for DAC: "));
    Serial.println("0x" + String(dac, HEX));

    byte r = 0;

    Serial.print(F("Register 07: "));
    r = i2c->ReadRegister(dac, 0x07);
    Serial.println(r, BIN);
    r = 0;

    Serial.print(F("Register 12: "));
    r = i2c->ReadRegister(dac, 0x0C);
    Serial.println(r, BIN);
    r = 0;

    Serial.print(F("Register 13: "));
    r = i2c->ReadRegister(dac, 0x0D);
    Serial.println(r, BIN);
    r = 0;

    Serial.print(F("Register 37: "));
    r = i2c->ReadRegister(dac, 0x25);
    Serial.println(r, BIN);
    r = 0;

    Serial.print(F("Register 37 (dec): "));
    r = i2c->ReadRegister(dac, 37);
    Serial.println(r, BIN);
    r = 0;

    Serial.print(F("Register 64: "));
    r = i2c->ReadRegister(dac, 0x40);
    Serial.println(r, BIN);
    r = 0;

    Serial.print(F("Register 100: "));
    r = i2c->ReadRegister(dac, 0x64);
    Serial.println(r, BIN);
    r = 0;
}
