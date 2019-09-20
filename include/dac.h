#ifndef _DAC_H
#define _DAC_H

#include <globals.h>
#include <i2c.h>
#include <Arduino.h>

// Set whether you will be using a power on/off relay. Comment out ALWAYSON if you will be using a remote for power on/off.
// #define ALWAYSON
#define TOTALINPUTS 5 // Total number of inputs (from 0 to...).
#define MCLK 10 // Value of Clock used (in 10s of MHz). 10 = 100MHz.

// Structure that holds the main parameters for each of the inputs.
struct DAC_Input
{
    int filter_val;         // Filter number.
    int IIR_val;            // IIR filter number.
    int DPLLPCM1;           // DPLL value for 768KHz.
    int DPLLPCM2;           // DPLL value for 705.6KHz.
    int DPLLPCM3;           // DPLL value for 384KHz.
    int DPLLPCM4;           // DPLL value for 352.8KHz or DoP 128.
    int DPLLPCM5;           // DPLL value for 192KHz.
    int DPLLPCM6;           // DPLL value for 176.4KHz or DoP 64.
    int DPLLPCM7;           // DPLL value for 96KHz.
    int DPLLPCM8;           // DPLL value for 88.2KHz.
    int DPLLPCM9;           // DPLL value for 48KHz.
    int DPLLPCM10;          // DPLL value for 44.1KHz.
    int DPLLPCM11;          // DPLL value for 32KHz.
    int DPLLDSD1;           // DPLL value for native DSD1024.
    int DPLLDSD2;           // DPLL value for native DSD512.
    int DPLLDSD3;           // DPLL value for native DSD256.
    int DPLLDSD4;           // DPLL value for native DSD128.
    int DPLLDSD5;           // DPLL value for native DSD64.
    int lockspeed_val;      // Lock speed setting.
    bool dither_val;        // Dither enabled or disabled.
    bool jitter_elim_val;   // Jitter Eliminator enabled or disabled.
    bool bypass_osf_val;    // OSF enabled or disabled.
    bool deemph_bypass_val; // De-emphasis enabled or disabled.
    bool auto_deemph_val;   // Auto de-emphasis enabled or disabled.
    int deemph_sel_val;     // De-emphasis filter selection.
};

class DAC
{

public:
    DAC();
    void setup();

    byte getDacAddress();

    void setInput();
    void setVol();
    void setFilter();
    void setIIR();
    void setMute();
    void setDPLLPCM();
    void setDPLLDSD();
    void setLockSpeed();
    void setDither();
    void setJitterElim();
    void setOSF();
    void setDeemphasis();
    void getSR();
    void getLockStatus();

    void PrintDebugRegAll(byte dac);
    void PrintDebugReg(byte dac);
};

#endif