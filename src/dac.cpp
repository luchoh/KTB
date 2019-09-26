#include <dac.h>

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
}

void DAC::setup()
{
  Serial.println(F("Setting up DAC"));
  DPLL_PCM = 5;
  DPLL_DSD = 5;
  i2c->ChangeBit(ES9038Q2M_ADDRESS, ES9038Q2M_SYSTEM_SETTING, 0, 1); // Put DAC in RESET mode
  delay(500);                          // Wait for 500ms
  i2c->ChangeBit(ES9038Q2M_ADDRESS, ES9038Q2M_SYSTEM_SETTING, 0, 0); // Take DAC out of RESET mode
  mute();                              // Put DAC in mute

  i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, 0x09, B01000000, B11110000); // Setup GPIO4 ?
  i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_VOLUME1, B00000100, B00000100); // Set DAC up for Stereo Mode
  i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_ADC_FBQ_SCALE1_1, B00000000, B10000000); // Enable DoP support
  unmute();                                                     // Take DAC out of mute

  DAC::setInput(0);
  DAC::setVol(55);
}

byte DAC::getDacAddress()
{
  return ES9038Q2M_ADDRESS;
}

void DAC::setVol(int level)
{
}

void DAC::initDAC()
{
  Serial.println(F("Setting up DAC"));
  // i2c->WriteRegister(ES9038Q2M_ADDRESS, ES9038Q2M_INPUT_CONFIG, 0x81);
  i2c->WriteRegister(ES9038Q2M_ADDRESS, ES9038Q2M_INPUT_CONFIG, 0xCC);
  i2c->WriteRegister(ES9038Q2M_ADDRESS, ES9038Q2M_GPIO_CONFIG, 0x98);
  // i2c->WriteRegister(ES9038Q2M_ADDRESS, ES9038Q2M_GPIO_CONFIG, 0xDD);
  i2c->WriteRegister(ES9038Q2M_ADDRESS, 0x0a, 0x02);
  i2c->WriteRegister(ES9038Q2M_ADDRESS, ES9038Q2M_SPDIF_SELECT, 0x30);
  i2c->WriteRegister(ES9038Q2M_ADDRESS, ES9038Q2M_VOLUME1, 0x00);
  i2c->WriteRegister(ES9038Q2M_ADDRESS, ES9038Q2M_VOLUME2, 0x00);
  i2c->WriteRegister(ES9038Q2M_ADDRESS, ES9038Q2M_INPUT_SEL, 0x50);
  i2c->WriteRegister(ES9038Q2M_ADDRESS, 0x09, 0x22);
  i2c->WriteRegister(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, 0x5f);
  i2c->WriteRegister(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, 0x00);
  i2c->WriteRegister(ES9038Q2M_ADDRESS, ES9038Q2M_SOFT_START, 0x8a);
  i2c->WriteRegister(ES9038Q2M_ADDRESS, ES9038Q2M_MASTERTRIM3, 0x70);
  i2c->WriteRegister(ES9038Q2M_ADDRESS, ES9038Q2M_2_HARMONIC_COMPENSATION_0, ES9038Q2M_3_HARMONIC_COMPENSATION_0);
  i2c->WriteRegister(ES9038Q2M_ADDRESS, ES9038Q2M_3_HARMONIC_COMPENSATION_0, 0xba);
  i2c->WriteRegister(ES9038Q2M_ADDRESS, ES9038Q2M_3_HARMONIC_COMPENSATION_1, 0xff);
}

int DAC::getMute()
{
  // get the register value
  return 0;
}

void DAC::mute()
{
  setMute(1);
}

void DAC::unmute()
{
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
    i2c->ChangeBit(ES9038Q2M_ADDRESS, ES9038Q2M_FILTER, 0, 0);
    Serial.println(F("DAC unmuted"));
  }
  else if (mute == 1)
  {
    i2c->ChangeBit(ES9038Q2M_ADDRESS, ES9038Q2M_FILTER, 0, 1);
    Serial.println(F("DAC muted"));
  }
}

void DAC::setAutoSignalDetection(bool autosignal, bool spdif)
{
  if (autosignal)
  {
    i2c->ChangeBit(ES9038Q2M_ADDRESS, ES9038Q2M_INPUT_CONFIG, 3, 1); // Enable auto signal detection
    i2c->ChangeBit(ES9038Q2M_ADDRESS, ES9038Q2M_INPUT_CONFIG, 2, 1); // Enable auto signal detection
  }
  else
  {
    i2c->ChangeBit(ES9038Q2M_ADDRESS, ES9038Q2M_INPUT_CONFIG, 3, 0); // Enable auto signal detection
    i2c->ChangeBit(ES9038Q2M_ADDRESS, ES9038Q2M_INPUT_CONFIG, 2, 0); // Enable auto signal detection
  }
  if (spdif)
  {
    i2c->ChangeBit(ES9038Q2M_ADDRESS, ES9038Q2M_INPUT_CONFIG, 1, 0); // Set ipt signal type to spdif
    i2c->ChangeBit(ES9038Q2M_ADDRESS, ES9038Q2M_INPUT_CONFIG, 0, 1); // Set ipt signal type to spdif
  } else {
    i2c->ChangeBit(ES9038Q2M_ADDRESS, ES9038Q2M_INPUT_CONFIG, 1, 0); // Set ipt signal type to spdif
    i2c->ChangeBit(ES9038Q2M_ADDRESS, ES9038Q2M_INPUT_CONFIG, 0, 0); // Set ipt signal type to spdif
  }
  Serial.print(F("Auto signal detection (Register 1): ")); 
  Serial.println(String(i2c->ReadRegister(ES9038Q2M_ADDRESS, ES9038Q2M_INPUT_CONFIG), BIN));
}

void DAC::setGPIO(int data)
{
  i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_SPDIF_SELECT, data, B11110000);
  Serial.print(F("GPIO Set to: ")); 
  Serial.println(String(i2c->ReadRegister(ES9038Q2M_ADDRESS, ES9038Q2M_SPDIF_SELECT), BIN));
}

// ----------------------------------------------  Function that selects the ipt and applies ipt-specific settings.
void DAC::setInput(int ipt)
{
  // int filter = 0;
  if (ipt == 0) // Select I2S/DSD ipt 1 with Auto detection
  {
    setGPIO(B00000000);
    setAutoSignalDetection(true, false);
  }
  else if (ipt == 1) // Select I2S/DSD ipt 2 with Auto detection
  {
    setGPIO(B00000000);
    setAutoSignalDetection(true, false);
  }
  else if (ipt == 2) // Select s/pdif from DATA1 pin
  {
    setGPIO(B00010000);
    setAutoSignalDetection(false, true);
  }
  else if (ipt == 3) // Select s/pdif from DATA2 pin
  {
    setGPIO(B00100000);
    setAutoSignalDetection(false, true);
  }
  else if (ipt == 4) // Select s/pdif from DATA3 pin
  {
    setGPIO(B00110000);
    setAutoSignalDetection(false, true);
  }
  else if (ipt == 5) // Select s/pdif from DATA4 pin
  {
    setGPIO(B01000000);
    setAutoSignalDetection(false, true);
  }
  // setFilter(Input[ipt].filter_val);
  // setIIR(Input[ipt].IIR_val);
  // setLockSpeed(Input[ipt].lockspeed_val);
  // setDither(Input[1].dither_val);
  // setJitterElim(Input[1].jitter_elim_val);
  // filter = Input[0].filter_val;
  // setFilter(filter);
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

  // filter = Input[2].filter_val;
  // setFilter(filter);
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

  // filter = Input[3].filter_val;
  // setFilter(filter);
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

  // filter = Input[4].filter_val;
  // setFilter(filter);
  // IIR = Input[4].IIR_val;
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

  // filter = Input[5].filter_val;
  // setFilter(filter);
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
  Serial.print(F("Input set to: "));
  Serial.println(ipt);
  // wait(500);
  Serial.print(F("Register 64: "));
  Serial.println(String(i2c->ReadRegister(ES9038Q2M_ADDRESS, ES9038Q2M_Chip_ID), BIN));
}

int DAC::getFilter()
{
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
      i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_FILTER, B00000000, B11100000);
    }
    else if (filter == 1)
    {
      i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_FILTER, B00100000, B11100000);
    }
    else if (filter == 2)
    {
      i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_FILTER, B01000000, B11100000);
    }
    else if (filter == 3)
    {
      i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_FILTER, B01100000, B11100000);
    }
    else if (filter == 4)
    {
      i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_FILTER, B10000000, B11100000);
    }
    else if (filter == 5)
    {
      i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_FILTER, B11000000, B11100000);
    }
    else if (filter == 6)
    {
      i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_FILTER, B11100000, B11100000);
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
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B00000000, B11110000);
  }
  else if (dpll_pcm == 1)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B00010000, B11110000);
  }
  else if (dpll_pcm == 2)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B00110000, B11110000);
  }
  else if (dpll_pcm == 3)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B01010000, B11110000);
  }
  else if (dpll_pcm == 4)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B01110000, B11110000);
  }
  else if (dpll_pcm == 5)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B10010000, B11110000);
  }
  else if (dpll_pcm == 6)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B10110000, B11110000);
  }
  else if (dpll_pcm == 7)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B11010000, B11110000);
  }
  else if (dpll_pcm == 8)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B11110000, B11110000);
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
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B00000000, B00001111);
  }
  else if (dpl_dsd == 1)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B00000001, B00001111);
  }
  else if (dpl_dsd == 2)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B00000011, B00001111);
  }
  else if (dpl_dsd == 3)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B00000101, B00001111);
  }
  else if (dpl_dsd == 4)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B00000111, B00001111);
  }
  else if (dpl_dsd == 5)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B00001001, B00001111);
  }
  else if (dpl_dsd == 6)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B00001010, B00001111);
  }
  else if (dpl_dsd == 7)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B00001011, B00001111);
  }
  else if (dpl_dsd == 8)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B00001101, B00001111);
  }
  else if (dpl_dsd == 9)
  {
    i2c->ChangeMultipleBits(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL, B00001111, B00001111);
  }
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
    r = i2c->ReadRegister(ES9038Q2M_ADDRESS, i);
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

  Serial.print(F("Register 01: "));
  r = i2c->ReadRegister(ES9038Q2M_ADDRESS, ES9038Q2M_INPUT_CONFIG);
  Serial.println(r, BIN);
  r = 0;

  Serial.print(F("Register 07: "));
  r = i2c->ReadRegister(ES9038Q2M_ADDRESS, ES9038Q2M_FILTER);
  Serial.println(r, BIN);
  r = 0;

  Serial.print(F("Register 08: "));
  r = i2c->ReadRegister(ES9038Q2M_ADDRESS, ES9038Q2M_GPIO_CONFIG);
  Serial.println(r, BIN);
  r = 0;

  Serial.print(F("Register 11: "));
  r = i2c->ReadRegister(ES9038Q2M_ADDRESS, ES9038Q2M_SPDIF_SELECT);
  Serial.println(r, BIN);
  r = 0;

  Serial.print(F("Register 12: "));
  r = i2c->ReadRegister(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL);
  Serial.println(r, BIN);
  r = 0;

  Serial.print(F("Register 13: "));
  r = i2c->ReadRegister(ES9038Q2M_ADDRESS, ES9038Q2M_DPLL);
  Serial.println(r, BIN);
  r = 0;

  Serial.print(F("Register 14: "));
  r = i2c->ReadRegister(ES9038Q2M_ADDRESS, ES9038Q2M_SOFT_START);
  Serial.println(r, BIN);
  r = 0;

  Serial.print(F("Register 15: "));
  r = i2c->ReadRegister(ES9038Q2M_ADDRESS, ES9038Q2M_VOLUME1);
  Serial.println(r, BIN);
  r = 0;

  Serial.print(F("Register 16: "));
  r = i2c->ReadRegister(ES9038Q2M_ADDRESS, ES9038Q2M_VOLUME2);
  Serial.println(r, BIN);
  r = 0;

  Serial.print(F("Register 20: "));
  r = i2c->ReadRegister(ES9038Q2M_ADDRESS, ES9038Q2M_MASTERTRIM3);
  Serial.println(r, BIN);
  r = 0;

  Serial.print(F("Register 21: "));
  r = i2c->ReadRegister(ES9038Q2M_ADDRESS, ES9038Q2M_INPUT_SEL);
  Serial.println(r, BIN);
  r = 0;

  Serial.print(F("Register 37: "));
  r = i2c->ReadRegister(ES9038Q2M_ADDRESS, ES9038Q2M_NCO_3);
  Serial.println(r, BIN);
  r = 0;

  Serial.print(F("Register 37 (dec): "));
  r = i2c->ReadRegister(ES9038Q2M_ADDRESS, 37);
  Serial.println(r, BIN);
  r = 0;

  Serial.print(F("Register 64: "));
  r = i2c->ReadRegister(ES9038Q2M_ADDRESS, ES9038Q2M_Chip_ID);
  Serial.println(r, BIN);
  r = 0;

  Serial.print(F("Register 100: "));
  r = i2c->ReadRegister(ES9038Q2M_ADDRESS, 0x64);
  Serial.println(r, BIN);
  r = 0;
}
