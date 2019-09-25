#include <dac.h>

byte dacAddr = 0x48; // address of DAC chip
int dac_chip = 0;    // Type of DAC chip for auto detection. 0 = ES9038Pro, 1 = ES9028Pro

#ifdef ALWAYSON
bool poweron = true; // Variable to hold whether the DAC is in power-on state or not
#else
bool poweron = false; // Variable to hold whether the DAC is in power-on state or not
#endif

int IIR = 0;
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
DAC_Input Input[TOTALINPUTS];

DAC::DAC()
{
    i2c->setup();
    
    if (poweron == true)
    {
        DPLL_PCM = 5;
        DPLL_DSD = 5;
        DAC::setup(); // Initialize the DAC chip
        // setInput();   // Set the ipt to the default ipt
        //main_disp(true);
    }
}

void DAC::setup()
{
    Serial.print(F("Setting up DAC"));
    i2c->ChangeBit(dacAddr, 0x00, 0, 1); // Put DAC in RESET mode
    delay(500);                          // Wait for 500ms
    i2c->ChangeBit(dacAddr, 0x00, 0, 0); // Take DAC out of RESET mode
    mute();                  // Put DAC in mute

    // i2c->ChangeMultipleBits(dacAddr, 0x09, B01000000, B11110000); // Setup GPIO4 ?
    i2c->ChangeMultipleBits(dacAddr, 0x0F, B00000100, B00000100); // Set DAC up for Stereo Mode
    i2c->ChangeMultipleBits(dacAddr, 0x36, B00000000, B10000000); // Enable DoP support
    unmute();                  // Take DAC out of mute

    DAC::setInput(0);
    DAC::setVol(55);
}

byte DAC::getDacAddress()
{
    return dacAddr;
}

void DAC::setVol(int level) {

}


int DAC::getMute() {
    // get the register value
    return 0;
}

void DAC::mute() {
    setMute(1);
}

void DAC::unmute() {
    setMute(0);
}
// -----------------------------  Function to mute the DAC
/* 0 = not mute
 * 1 = mute
 */
void DAC::setMute(int mute)
{
  if (mute == 0)
  {
    i2c->ChangeBit(dacAddr, 0x07, 0, 0);
    Serial.println(F("DAC unmuted"));
  }
  else if (mute == 1)
  {
    i2c->ChangeBit(dacAddr, 0x07, 0, 1);
    Serial.println(F("DAC muted"));
  }
}

void DAC::setAutoSignalDetection(bool autosignal, bool spdif) {
    if(autosignal) {
        i2c->ChangeBit(dacAddr, 0x01, 3, 1);     // Enable auto signal detection
        i2c->ChangeBit(dacAddr, 0x01, 2, 1);     // Enable auto signal detection
    } else {
        i2c->ChangeBit(dacAddr, 0x01, 3, 0);     // Enable auto signal detection
        i2c->ChangeBit(dacAddr, 0x01, 2, 0);     // Enable auto signal detection
    }
    if(spdif) {
        i2c->ChangeBit(dacAddr, 0x01, 1, 0);    // Set ipt signal type to spdif
        i2c->ChangeBit(dacAddr, 0x01, 0, 1);    // Set ipt signal type to spdif
    }
}

void DAC::setGPIO(int data) {
    i2c->ChangeMultipleBits(dacAddr, 0x0B, data, B11110000);
}

// ----------------------------------------------  Function that selects the ipt and applies ipt-specific settings.
void DAC::setInput(int ipt)
{
    int filter = 0;
  if (ipt == 0) // Select I2S/DSD ipt 1 with Auto detection
  {
    setAutoSignalDetection(true, false);
    setGPIO(B00000000);
  }
  else if (ipt == 1) // Select I2S/DSD ipt 2 with Auto detection
  {
    setAutoSignalDetection(true, false);
    setGPIO(B00000000);
  }
  else if (ipt == 2) // Select s/pdif from DATA1 pin
  {
    setAutoSignalDetection(false, true);
    setGPIO(B00010000);
  }
  else if (ipt == 3) // Select s/pdif from DATA2 pin
  {
    setAutoSignalDetection(false, true);
    setGPIO(B00100000);
  }
  else if (ipt == 4) // Select s/pdif from DATA3 pin
  {
    setAutoSignalDetection(false, true);
    setGPIO(B00110000);
  }
  else if (ipt == 5) // Select s/pdif from DATA4 pin
  {
    setAutoSignalDetection(false, true);
    setGPIO(B01000000);
  }
  setFilter(Input[ipt].filter_val);
  setIIR(Input[ipt].IIR_val);
  setLockSpeed(Input[ipt].lockspeed_val);
  // setDither(Input[1].dither_val);
  // setJitterElim(Input[1].jitter_elim_val);
    filter = Input[0].filter_val;
    setFilter(filter);
    // IIR = Input[0].IIR_val;
    // setIIR();
    // LockSpeed = Input[0].lockspeed_val;
    // setLockSpeed();
    // Dither = Input[0].dither_val;
    // setDither();
    // JitterElim = Input[0].jitter_elim_val;
    // setJitterElim();
    // bypass_osf = Input[0].bypass_osf_val;
    // deemph_bypass = Input[0].deemph_bypass_val;
    // setOSF();
    // auto_deemph = Input[0].auto_deemph_val;
    // deemph_sel = Input[0].deemph_sel_val;
    // setDeemphasis();
  
    
    // bypass_osf = Input[1].bypass_osf_val;
    // deemph_bypass = Input[1].deemph_bypass_val;
    // setOSF();
    // auto_deemph = Input[1].auto_deemph_val;
    // deemph_sel = Input[1].deemph_sel_val;
    // setDeemphasis();
  
    filter = Input[2].filter_val;
    setFilter(filter);
    // IIR = Input[2].IIR_val;
    // setIIR();
    // LockSpeed = Input[2].lockspeed_val;
    // setLockSpeed();
    // Dither = Input[2].dither_val;
    // setDither();
    // JitterElim = Input[2].jitter_elim_val;
    // setJitterElim();
    // bypass_osf = Input[2].bypass_osf_val;
    // deemph_bypass = Input[2].deemph_bypass_val;
    // setOSF();
    // auto_deemph = Input[2].auto_deemph_val;
    // deemph_sel = Input[2].deemph_sel_val;
    // setDeemphasis();
  
    filter = Input[3].filter_val;
    setFilter(filter);
    // IIR = Input[3].IIR_val;
    // setIIR();
    // LockSpeed = Input[3].lockspeed_val;
    // setLockSpeed();
    // Dither = Input[3].dither_val;
    // setDither();
    // JitterElim = Input[3].jitter_elim_val;
    // setJitterElim();
    // bypass_osf = Input[3].bypass_osf_val;
    // deemph_bypass = Input[3].deemph_bypass_val;
    // setOSF();
    // auto_deemph = Input[3].auto_deemph_val;
    // deemph_sel = Input[3].deemph_sel_val;
    // setDeemphasis();
  
    filter = Input[4].filter_val;
    setFilter(filter);
    IIR = Input[4].IIR_val;
    // setIIR();
    // LockSpeed = Input[4].lockspeed_val;
    // setLockSpeed();
    // Dither = Input[4].dither_val;
    // setDither();
    // JitterElim = Input[4].jitter_elim_val;
    // setJitterElim();
    // bypass_osf = Input[4].bypass_osf_val;
    // deemph_bypass = Input[4].deemph_bypass_val;
    // setOSF();
    // auto_deemph = Input[4].auto_deemph_val;
    // deemph_sel = Input[4].deemph_sel_val;
    // setDeemphasis();
  
    filter = Input[5].filter_val;
    setFilter(filter);
    // IIR = Input[5].IIR_val;
    // setIIR();
    // LockSpeed = Input[5].lockspeed_val;
    // setLockSpeed();
    // Dither = Input[5].dither_val;
    // setDither();
    // JitterElim = Input[5].jitter_elim_val;
    // setJitterElim();
    // bypass_osf = Input[5].bypass_osf_val;
    // deemph_bypass = Input[5].deemph_bypass_val;
    // setOSF();
    // auto_deemph = Input[5].auto_deemph_val;
    // deemph_sel = Input[5].deemph_sel_val;
    // setDeemphasis();

#ifdef DEBUG
  Serial.print(F("Input set to: "));
#endif
#ifdef DEBUG
  Serial.println(ipt);
#endif
}

int DAC::getFilter(){
    // get it from the DAC register
    return 0;
}

// -----------------------------  Function that sets the filter
/* 0 = Fast Roll-off, Linear Phase Filter
 * 1 = Slow Roll-off, Linear Phase Filter 
 * 2 = Fast Roll-off, Minimum Phase Filter
 * 3 = Slow Roll-off, Minimum Phase
 * 4 = Apodizing, Fast Roll-off, Linear Phase 
 * 5 = Hybrid, Fast Roll-off, Minimum Phase 
 * 6 = Brickwall 
 */
void DAC::setFilter(int filter)
{
  if ((filter >= 0) && (filter <= 6))
  {
    if (filter == 0)
    {
      i2c->ChangeMultipleBits(dacAddr, 0x07, B00000000, B11100000);
    }
    else if (filter == 1)
    {
      i2c->ChangeMultipleBits(dacAddr, 0x07, B00100000, B11100000);
    }
    else if (filter == 2)
    {
      i2c->ChangeMultipleBits(dacAddr, 0x07, B01000000, B11100000);
    }
    else if (filter == 3)
    {
      i2c->ChangeMultipleBits(dacAddr, 0x07, B01100000, B11100000);
    }
    else if (filter == 4)
    {
      i2c->ChangeMultipleBits(dacAddr, 0x07, B10000000, B11100000);
    }
    else if (filter == 5)
    {
      i2c->ChangeMultipleBits(dacAddr, 0x07, B11000000, B11100000);
    }
    else if (filter == 6)
    {
      i2c->ChangeMultipleBits(dacAddr, 0x07, B11100000, B11100000);
    }
    Serial.print(F("Filter set to: "));
    Serial.println(filter);
  }
  else
  {
    Serial.print(F("Invalid Filter Number ("));
    Serial.print(filter);
    Serial.println(F(") re-initializing EEPROM and rebooting."));
    delay(200);
    // initEEPROM();
    delay(200);
    // initEEPROM();
    delay(200);
    // DeviceInit();
  }
}


// -----------------------------  Function to set the DPLL for I2S / SPDIF
/* 0 = DPLL Off
 * 1 = Lowest (1)
 * 2 = Low (3)
 * 3 = Default (5)
 * 4 = Med-Low (7)
 * 5 = Medium (9)
 * 6 = High (11)
 * 7 = Higher (13)
 * 8 = Highest (15)
 */
void setDPLLPCM(int dpll_pcm = 5)
{
  if (dpll_pcm == 0)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B00000000, B11110000);
  }
  else if (dpll_pcm == 1)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B00010000, B11110000);
  }
  else if (dpll_pcm == 2)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B00110000, B11110000);
  }
  else if (dpll_pcm == 3)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B01010000, B11110000);
  }
  else if (dpll_pcm == 4)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B01110000, B11110000);
  }
  else if (dpll_pcm == 5)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B10010000, B11110000);
  }
  else if (dpll_pcm == 6)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B10110000, B11110000);
  }
  else if (dpll_pcm == 7)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B11010000, B11110000);
  }
  else if (dpll_pcm == 8)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B11110000, B11110000);
  }
}

// -----------------------------  Function to set the DPLL for DSD
/* 0 = DPLL Off
 * 1 = Lowest (1)
 * 2 = Low (3)
 * 3 = Med-Low (5)
 * 4 = Medium (7)
 * 5 = Med-High (9)
 * 6 = Default (10)
 * 7 = High (11)
 * 8 = Higher (13)
 * 9 = Highest (15)
 */

void setDPLLDSD(int dpl_dsd = 5)
{
  if (dpl_dsd == 0)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B00000000, B00001111);  
  }
  else if (dpl_dsd == 1)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B00000001, B00001111);
  }
  else if (dpl_dsd == 2)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B00000011, B00001111);
  }
  else if (dpl_dsd == 3)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B00000101, B00001111);
  }
  else if (dpl_dsd == 4)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B00000111, B00001111);
  }
  else if (dpl_dsd == 5)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B00001001, B00001111);
  }
  else if (dpl_dsd == 6)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B00001010, B00001111);
  }
  else if (dpl_dsd == 7)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B00001011, B00001111);
  }
  else if (dpl_dsd == 8)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B00001101, B00001111);
  }
  else if (dpl_dsd == 9)
  {
    i2c->ChangeMultipleBits(dacAddr, 0x0C, B00001111, B00001111);
  }
}

#ifdef SKIPCOMPILATION


// -----------------------------  Function to determine Sampling Rate
/* sig_type:
 * 0 = DOP
 * 1 = SPDIF
 * 2 = I2S
 * 3 = DSD
*/
void getSR()
{
  volatile unsigned long DPLLNum; // Variable to hold DPLL value
  volatile unsigned long RegVal;  // Variable to hold Register values

  DPLLNum = 0;
  // Reading the 4 registers of DPLL one byte at a time and stuffing into a single 32-bit number
  DPLLNum |= i2c->ReadRegister(dacAddr, 0x45);
  DPLLNum <<= 8;
  DPLLNum |= i2c->ReadRegister(dacAddr, 0x44);
  DPLLNum <<= 8;
  DPLLNum |= i2c->ReadRegister(dacAddr, 0x43);
  DPLLNum <<= 8;
  DPLLNum |= i2c->ReadRegister(dacAddr, 0x42);

  Serial.print(F("Raw DPLL number: "));

  Serial.println(DPLLNum);


  /*
  if (sig_type == 1)
  {
    DPLLNum*=20;      // Calculate SR for SPDIF 
    DPLLNum/=859;     // Calculate SR for SDPIF
  }
  else 
  {                  // Different calculation for SPDIF and I2S/DSD
    DPLLNum*=4;      // Calculate SR for I2S/DSD
    DPLLNum/=10995;  // Calculate SR for I2S/DSD
  }
  */

  long sampleRate = 0;
  sampleRate = (DPLLNum * MCLK) / 42940;

  Serial.print(F("Sample Rate: "));

  Serial.println(sampleRate);


  if (DPLLNum < 90000) // Adjusting because in integer operation, the residual is truncated
    DPLLNum += 1;
  else if (DPLLNum < 190000)
    DPLLNum += 2;
  else if (DPLLNum < 350000)
    DPLLNum += 3;
  else
    DPLLNum += 4;

  if ((sig_type == 0) || (sig_type == 3)) // SR calculation for DSD signals (either native or DoP)
  {
    if (sampleRate > 461520)
    {
      SR = 0;
      if (ipt == 0)
      {
        setDPLLDSD(15);
      }
      else if (ipt == 1)
      {
        setDPLLDSD(15);
      }
      else if (ipt == 2)
      {
        setDPLLDSD(15);
      }
      else if (ipt == 3)
      {
        setDPLLDSD(15);
      }
      else if (ipt == 4)
      {
        setDPLLDSD(15);
      }
      else if (ipt == 5)
      {
        setDPLLDSD(15);
      }
      Serial.println(F("Invalid DSD, DPLL set to 15"));

    }
    else if (sampleRate > 451000)
    {
      SR = 1;
      if (ipt == 0)
      {
        setDPLLDSD(DAC::Input[0].DPLLDSD1);
      }
      else if (ipt == 1)
      {
        setDPLLDSD(DAC::Input[1].DPLLDSD1);
      }
      else if (ipt == 2)
      {
        setDPLLDSD(DAC::Input[2].DPLLDSD1);
      }
      else if (ipt == 3)
      {
        setDPLLDSD(DAC::Input[3].DPLLDSD1);
      }
      else if (ipt == 4)
      {
        setDPLLDSD(DAC::Input[4].DPLLDSD1);
      }
      else if (ipt == 5)
      {
        setDPLLDSD(DAC::Input[5].DPLLDSD1);
      }
      Serial.println(F("44.8 MHz DSD"));

    }
    else if (sampleRate > 225700)
    {
      SR = 2;
      if (ipt == 0)
      {
        setDPLLDSD(DAC::Input[0].DPLLDSD2);
      }
      else if (ipt == 1)
      {
        setDPLLDSD(DAC::Input[1].DPLLDSD2);
      }
      else if (ipt == 2)
      {
        setDPLLDSD(DAC::Input[2].DPLLDSD2);
      }
      else if (ipt == 3)
      {
        setDPLLDSD(DAC::Input[3].DPLLDSD2);
      }
      else if (ipt == 4)
      {
        setDPLLDSD(DAC::Input[4].DPLLDSD2);
      }
      else if (ipt == 5)
      {
        setDPLLDSD(DAC::Input[5].DPLLDSD2);
      }
      Serial.println(F("22.4 MHz DSD"));

    }
    else if (sampleRate > 112000)
    {
      SR = 3;
      if (ipt == 0)
      {
        setDPLLDSD(DAC::Input[0].DPLLDSD3);
      }
      else if (ipt == 1)
      {
        setDPLLDSD(DAC::Input[1].DPLLDSD3);
      }
      else if (ipt == 2)
      {
        setDPLLDSD(DAC::Input[2].DPLLDSD3);
      }
      else if (ipt == 3)
      {
        setDPLLDSD(DAC::Input[3].DPLLDSD3);
      }
      else if (ipt == 4)
      {
        setDPLLDSD(DAC::Input[4].DPLLDSD3);
      }
      else if (ipt == 5)
      {
        setDPLLDSD(DAC::Input[5].DPLLDSD3);
      }
      Serial.println(F("11.2 MHz DSD"));

    }
    else if (sampleRate > 56000)
    {
      SR = 4;
      if (ipt == 0)
      {
        setDPLLDSD(DAC::Input[0].DPLLDSD4);
      }
      else if (ipt == 1)
      {
        setDPLLDSD(DAC::Input[1].DPLLDSD4);
      }
      else if (ipt == 2)
      {
        setDPLLDSD(DAC::Input[2].DPLLDSD4);
      }
      else if (ipt == 3)
      {
        setDPLLDSD(DAC::Input[3].DPLLDSD4);
      }
      else if (ipt == 4)
      {
        setDPLLDSD(DAC::Input[4].DPLLDSD4);
      }
      else if (ipt == 5)
      {
        setDPLLDSD(DAC::Input[5].DPLLDSD4);
      }
      Serial.println(F("5.6 MHz DSD"));

    }
    else if ((sampleRate > 28000) || (sampleRate > 1700))
    {
      SR = 5;
      if (ipt == 0)
      {
        setDPLLDSD(DAC::Input[0].DPLLDSD5);
      }
      else if (ipt == 1)
      {
        setDPLLDSD(DAC::Input[1].DPLLDSD5);
      }
      else if (ipt == 2)
      {
        setDPLLDSD(DAC::Input[2].DPLLDSD5);
      }
      else if (ipt == 3)
      {
        setDPLLDSD(DAC::Input[3].DPLLDSD5);
      }
      else if (ipt == 4)
      {
        setDPLLDSD(DAC::Input[4].DPLLDSD5);
      }
      else if (ipt == 5)
      {
        setDPLLDSD(DAC::Input[5].DPLLDSD5);
      }
      Serial.println(F("2.8 MHz DSD"));

    }
    else
    {
      SR = 0;
      if (ipt == 0)
      {
        setDPLLDSD(15);
      }
      else if (ipt == 1)
      {
        setDPLLDSD(15);
      }
      else if (ipt == 2)
      {
        setDPLLDSD(15);
      }
      else if (ipt == 3)
      {
        setDPLLDSD(15);
      }
      else if (ipt == 4)
      {
        setDPLLDSD(15);
      }
      else if (ipt == 5)
      {
        setDPLLDSD(15);
      }
      Serial.println(F("Invalid DSD, DPLL set to 15."));

    }
  }

  if (sig_type == 1 || sig_type == 2) // If signal is PCM (either SPDIF or I2S)
  {
    if (sampleRate > 7690)
    {
      SR = 0;
      if (ipt == 0)
      {
        setDPLLPCM(15);
      }
      else if (ipt == 1)
      {
        setDPLLPCM(15);
      }
      else if (ipt == 2)
      {
        setDPLLPCM(15);
      }
      else if (ipt == 3)
      {
        setDPLLPCM(15);
      }
      else if (ipt == 4)
      {
        setDPLLPCM(15);
      }
      else if (ipt == 5)
      {
        setDPLLPCM(15);
      }
      Serial.println(F("Invalid SR, DPLL set to 15."));

    }
    else if (sampleRate > 7675)
    {
      SR = 6;
      if (ipt == 0)
      {
        setDPLLPCM(DAC::Input[0].DPLLPCM1);
      }
      else if (ipt == 1)
      {
        setDPLLPCM(DAC::Input[1].DPLLPCM1);
      }
      else if (ipt == 2)
      {
        setDPLLPCM(DAC::Input[2].DPLLPCM1);
      }
      else if (ipt == 3)
      {
        setDPLLPCM(DAC::Input[3].DPLLPCM1);
      }
      else if (ipt == 4)
      {
        setDPLLPCM(DAC::Input[4].DPLLPCM1);
      }
      else if (ipt == 5)
      {
        setDPLLPCM(DAC::Input[5].DPLLPCM1);
      }
      Serial.println(F("768K PCM"));

    }
    else if (sampleRate > 7050)
    {
      SR = 7;
      if (ipt == 0)
      {
        setDPLLPCM(DAC::Input[0].DPLLPCM2);
      }
      else if (ipt == 1)
      {
        setDPLLPCM(DAC::Input[1].DPLLPCM2);
      }
      else if (ipt == 2)
      {
        setDPLLPCM(DAC::Input[2].DPLLPCM2);
      }
      else if (ipt == 3)
      {
        setDPLLPCM(DAC::Input[3].DPLLPCM2);
      }
      else if (ipt == 4)
      {
        setDPLLPCM(DAC::Input[4].DPLLPCM2);
      }
      else if (ipt == 5)
      {
        setDPLLPCM(DAC::Input[5].DPLLPCM2);
      }
      Serial.println(F("705.6K PCM"));

    }
    else if (sampleRate > 3835)
    {
      SR = 8;
      if (ipt == 0)
      {
        setDPLLPCM(DAC::Input[0].DPLLPCM3);
      }
      else if (ipt == 1)
      {
        setDPLLPCM(DAC::Input[1].DPLLPCM3);
      }
      else if (ipt == 2)
      {
        setDPLLPCM(DAC::Input[2].DPLLPCM3);
      }
      else if (ipt == 3)
      {
        setDPLLPCM(DAC::Input[3].DPLLPCM3);
      }
      else if (ipt == 4)
      {
        setDPLLPCM(DAC::Input[4].DPLLPCM3);
      }
      else if (ipt == 5)
      {
        setDPLLPCM(DAC::Input[5].DPLLPCM3);
      }
      Serial.println(F("384K PCM"));

    }
    else if (sampleRate > 3510)
    {
      SR = 9;
      if (ipt == 0)
      {
        setDPLLPCM(DAC::Input[0].DPLLPCM4);
      }
      else if (ipt == 1)
      {
        setDPLLPCM(DAC::Input[1].DPLLPCM4);
      }
      else if (ipt == 2)
      {
        setDPLLPCM(DAC::Input[2].DPLLPCM4);
      }
      else if (ipt == 3)
      {
        setDPLLPCM(DAC::Input[3].DPLLPCM4);
      }
      else if (ipt == 4)
      {
        setDPLLPCM(DAC::Input[4].DPLLPCM4);
      }
      else if (ipt == 5)
      {
        setDPLLPCM(DAC::Input[5].DPLLPCM4);
      }
      Serial.println(F("352.8K PCM"));

    }
    else if (sampleRate > 1910)
    {
      SR = 10;
      if (ipt == 0)
      {
        setDPLLPCM(DAC::Input[0].DPLLPCM5);
      }
      else if (ipt == 1)
      {
        setDPLLPCM(DAC::Input[1].DPLLPCM5);
      }
      else if (ipt == 2)
      {
        setDPLLPCM(DAC::Input[2].DPLLPCM5);
      }
      else if (ipt == 3)
      {
        setDPLLPCM(DAC::Input[3].DPLLPCM5);
      }
      else if (ipt == 4)
      {
        setDPLLPCM(DAC::Input[4].DPLLPCM5);
      }
      else if (ipt == 5)
      {
        setDPLLPCM(DAC::Input[5].DPLLPCM5);
      }
      Serial.println(F("192K PCM"));

    }
    else if (sampleRate > 1756)
    {
      SR = 11;
      if (ipt == 0)
      {
        setDPLLPCM(DAC::Input[0].DPLLPCM6);
      }
      else if (ipt == 1)
      {
        setDPLLPCM(DAC::Input[1].DPLLPCM6);
      }
      else if (ipt == 2)
      {
        setDPLLPCM(DAC::Input[2].DPLLPCM6);
      }
      else if (ipt == 3)
      {
        setDPLLPCM(DAC::Input[3].DPLLPCM6);
      }
      else if (ipt == 4)
      {
        setDPLLPCM(DAC::Input[4].DPLLPCM6);
      }
      else if (ipt == 5)
      {
        setDPLLPCM(DAC::Input[5].DPLLPCM6);
      }
      Serial.println(F("176.4K PCM"));

    }
    else if (sampleRate > 954)
    {
      SR = 12;
      if (ipt == 0)
      {
        setDPLLPCM(DAC::Input[0].DPLLPCM7);
      }
      else if (ipt == 1)
      {
        setDPLLPCM(DAC::Input[1].DPLLPCM7);
      }
      else if (ipt == 2)
      {
        setDPLLPCM(DAC::Input[2].DPLLPCM7);
      }
      else if (ipt == 3)
      {
        setDPLLPCM(DAC::Input[3].DPLLPCM7);
      }
      else if (ipt == 4)
      {
        setDPLLPCM(DAC::Input[4].DPLLPCM7);
      }
      else if (ipt == 5)
      {
        setDPLLPCM(DAC::Input[5].DPLLPCM7);
      }
      Serial.println(F("96K PCM"));

    }
    else if (sampleRate > 878)
    {
      SR = 13;
      if (ipt == 0)
      {
        setDPLLPCM(DAC::Input[0].DPLLPCM8);
      }
      else if (ipt == 1)
      {
        setDPLLPCM(DAC::Input[1].DPLLPCM8);
      }
      else if (ipt == 2)
      {
        setDPLLPCM(DAC::Input[2].DPLLPCM8);
      }
      else if (ipt == 3)
      {
        setDPLLPCM(DAC::Input[3].DPLLPCM8);
      }
      else if (ipt == 4)
      {
        setDPLLPCM(DAC::Input[4].DPLLPCM8);
      }
      else if (ipt == 5)
      {
        setDPLLPCM(DAC::Input[5].DPLLPCM8);
      }
      Serial.println(F("88.2K PCM"));

    }
    else if (sampleRate > 475)
    {
      SR = 14;
      if (ipt == 0)
      {
        setDPLLPCM(DAC::Input[0].DPLLPCM9);
      }
      else if (ipt == 1)
      {
        setDPLLPCM(DAC::Input[1].DPLLPCM9);
      }
      else if (ipt == 2)
      {
        setDPLLPCM(DAC::Input[2].DPLLPCM9);
      }
      else if (ipt == 3)
      {
        setDPLLPCM(DAC::Input[3].DPLLPCM9);
      }
      else if (ipt == 4)
      {
        setDPLLPCM(DAC::Input[4].DPLLPCM9);
      }
      else if (ipt == 5)
      {
        setDPLLPCM(DAC::Input[5].DPLLPCM9);
      }
      Serial.println(F("48K PCM"));

    }
    else if (sampleRate > 430)
    {
      SR = 15;
      if (ipt == 0)
      {
        setDPLLPCM(DAC::Input[0].DPLLPCM10);
      }
      else if (ipt == 1)
      {
        setDPLLPCM(DAC::Input[1].DPLLPCM10);
      }
      else if (ipt == 2)
      {
        setDPLLPCM(DAC::Input[2].DPLLPCM10);
      }
      else if (ipt == 3)
      {
        setDPLLPCM(DAC::Input[3].DPLLPCM10);
      }
      else if (ipt == 4)
      {
        setDPLLPCM(DAC::Input[4].DPLLPCM10);
      }
      else if (ipt == 5)
      {
        setDPLLPCM(DAC::Input[5].DPLLPCM10);
      }
      Serial.println(F("44.1K PCM"));

    }
    else if (sampleRate > 310)
    {
      SR = 16;
      if (ipt == 0)
      {
        setDPLLPCM(DAC::Input[0].DPLLPCM11);
      }
      else if (ipt == 1)
      {
        setDPLLPCM(DAC::Input[1].DPLLPCM11);
      }
      else if (ipt == 2)
      {
        setDPLLPCM(DAC::Input[2].DPLLPCM11);
      }
      else if (ipt == 3)
      {
        setDPLLPCM(DAC::Input[3].DPLLPCM11);
      }
      else if (ipt == 4)
      {
        setDPLLPCM(DAC::Input[4].DPLLPCM11);
      }
      else if (ipt == 5)
      {
        setDPLLPCM(DAC::Input[5].DPLLPCM11);
      }
      Serial.println(F("32K PCM"));

    }
  }
}
#endif

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
        r = i2c->ReadRegister(dacAddr, i);
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
    r = i2c->ReadRegister(dacAddr, 0x07);
    Serial.println(r, BIN);
    r = 0;

    Serial.print(F("Register 12: "));
    r = i2c->ReadRegister(dacAddr, 0x0C);
    Serial.println(r, BIN);
    r = 0;

    Serial.print(F("Register 13: "));
    r = i2c->ReadRegister(dacAddr, 0x0D);
    Serial.println(r, BIN);
    r = 0;

    Serial.print(F("Register 37: "));
    r = i2c->ReadRegister(dacAddr, 0x25);
    Serial.println(r, BIN);
    r = 0;

    Serial.print(F("Register 37 (dec): "));
    r = i2c->ReadRegister(dacAddr, 37);
    Serial.println(r, BIN);
    r = 0;

    Serial.print(F("Register 64: "));
    r = i2c->ReadRegister(dacAddr, 0x40);
    Serial.println(r, BIN);
    r = 0;

    Serial.print(F("Register 100: "));
    r = i2c->ReadRegister(dacAddr, 0x64);
    Serial.println(r, BIN);
    r = 0;
}
