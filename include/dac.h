#ifndef _DAC_H
#define _DAC_H

#include <globals.h>
#include <i2c.h>
#include <Arduino.h>

#define ES9038Q2M_SYSTEM_SETTING                0x00
#define ES9038Q2M_INPUT_CONFIG                  0x01
#define ES9038Q2M_MIXING                        0x02
#define ES9038Q2M_AUTOMUTE_TIME                 0x04
#define ES9038Q2M_AUTOMUTE_LEVEL                0x05
#define ES9038Q2M_DEEMP_DOP                     0x06
#define ES9038Q2M_FILTER                        0x07
#define ES9038Q2M_GPIO_CONFIG                   0x08
#define ES9038Q2M_MASTER_MODE                   0x0A
#define ES9038Q2M_SPDIF_SELECT                  0x0B
#define ES9038Q2M_DPLL                          0x0C
#define ES9038Q2M_THD_COMPENSATION              0x0D
#define ES9038Q2M_SOFT_START                    0x0E
#define ES9038Q2M_VOLUME1                       0x0F
#define ES9038Q2M_VOLUME2                       0x10
#define ES9038Q2M_MASTERTRIM0                   0x11
#define ES9038Q2M_MASTERTRIM1                   0x12
#define ES9038Q2M_MASTERTRIM2                   0x13
#define ES9038Q2M_MASTERTRIM3                   0x14
#define ES9038Q2M_INPUT_SEL                     0x15
#define ES9038Q2M_2_HARMONIC_COMPENSATION_0     0x16
#define ES9038Q2M_2_HARMONIC_COMPENSATION_1     0x17
#define ES9038Q2M_3_HARMONIC_COMPENSATION_0     0x18
#define ES9038Q2M_3_HARMONIC_COMPENSATION_1     0x19
#define ES9038Q2M_GENERAL_CONFIG_0              0x1B
#define ES9038Q2M_GPIO_INV                      0x1D
#define ES9038Q2M_CP_CLK_0                      0x1E
#define ES9038Q2M_CP_CLK_1                      0x1F
#define ES9038Q2M_INT_MASK                      0x21
#define ES9038Q2M_NCO_0                         0x22
#define ES9038Q2M_NCO_1                         0x23
#define ES9038Q2M_NCO_2                         0x24
#define ES9038Q2M_NCO_3                         0x25
#define ES9038Q2M_GENERAL_CONFIG_1              0x27
#define ES9038Q2M_program_FIR_ADDR              0x28
#define ES9038Q2M_program_FIR_DATA_0            0x29
#define ES9038Q2M_program_FIR_DATA_1            0x2A
#define ES9038Q2M_program_FIR_DATA_2            0x2B
#define ES9038Q2M_program_FIR_CONTROL           0x2C
#define ES9038Q2M_AUTO_CAL                      0x2D
#define ES9038Q2M_ADC_CONFIG                    0x2E

#define ES9038Q2M_ADC_FTR_SCALE_0               0x2F
#define ES9038Q2M_ADC_FTR_SCALE_1               0x30
#define ES9038Q2M_ADC_FTR_SCALE_2               0x31
#define ES9038Q2M_ADC_FTR_SCALE_3               0x32
#define ES9038Q2M_ADC_FTR_SCALE_4               0x33
#define ES9038Q2M_ADC_FTR_SCALE_5               0x34

#define ES9038Q2M_ADC_FBQ_SCALE1_0              0x35
#define ES9038Q2M_ADC_FBQ_SCALE1_1              0x36

#define ES9038Q2M_ADC_FBQ_SCALE2_0              0x37
#define ES9038Q2M_ADC_FBQ_SCALE2_1              0x38

#define ES9038Q2M_Chip_ID                       0x40

#define ES9038Q2M_ADDRESS                       0x48

#define ES9038Q2M_SYSCLK_MCLK_HZ                49152000

// Set whether you will be using a power on/off relay. Comment out ALWAYSON if you will be using a remote for power on/off.
// #define ALWAYSON
#define TOTALINPUTS 5 // Total number of inputs (from 0 to...).
#define MCLK 10       // Value of Clock used (in 10s of MHz). 10 = 100MHz.

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

    void setAutoSignalDetection(bool autosignal, bool spdif);
    void setGPIO(int mask);
    void setInput(int input);
    int getInput();
    void setVol(int level);
    int getFilter();
    void setFilter(int filter);
    void setIIR(int iir);
    int getMute();
    void mute();
    void unmute();
    void setMute(int mute);
    void setDPLLPCM();
    void setDPLLDSD();
    void setLockSpeed(int speed);
    void setDither();
    void setJitterElim();
    void setOSF();
    void setDeemphasis();
    void getSR();
    void getLockStatus();
    void initDAC();

    void PrintDebugRegAll(byte dac);
    void PrintDebugReg(byte dac);

private:
    int DPLL_PCM;
    int DPLL_DSD;
    DAC_Input Input[TOTALINPUTS]; // Declare structures of Inputs
};

#endif