#include <globals.h>
#include <dac.h>

DAC *dac;
I2C *daci2c;

// ----------------------------- Function that reads a character from the serial port ------------------------------------------------------------
char getCommand()
{
  char c = '\0';
  if (Serial.available())
  {
    c = Serial.read();
  }
  return c;
}

// ----------------------------- Function that displays available commands through the serial port -----------------------------------------------
void displayHelp()
{
  // if (dac_chip == 0)
  // {
  //   Serial.print("ES9038Pro");
  // }
  // else if (dac_chip == 1)
  // {
  //   Serial.print("ES9028Pro");
  // }
  // else if (dac_chip == 2)
  // {
  //   Serial.print("ES9028qm");
  // }
  Serial.println(F(" DAC Controller"));
  Serial.println();

  Serial.println(F("Press a to select Input 1"));

  Serial.println(F("Press b to select Input 2"));

  Serial.println(F("Press c to select Input 3"));

  Serial.println(F("Press d to set IIR bandwidth to 47.44KHz"));

  Serial.println(F("Press e to set IIR bandwidth to 50KHz"));

  Serial.println(F("Press f to set IIR bandwidth to 60KHz"));

  Serial.println(F("Press g to set IIR bandwidth to 70KHz"));

  Serial.println(F("Press h to unmute the DAC"));

  Serial.println(F("Press i to mute the DAC"));

  Serial.println(F("Press j to get Lock status and calculate the SR"));

  Serial.println(F("Press 0 to 6 to select filter"));

  Serial.println(F("Press y to display the important registers"));

  Serial.println(F("Press z to display this menu"));

  Serial.println(F("Press ! to restore EEPROM values to defaults"));

  Serial.println(F("Press @ to initialize the DAC"));

  Serial.println(F("Press ^ to test read EEPROM contents"));

  Serial.println(F("Press p to display all registers"));

  Serial.println();
}

void processCommand()
{
  // byte dacAddr = dac->getDacAddress();
  char command = getCommand();
  switch (command)
  {
  case 'a':
    // input = 0;
    // setInput();
    // main_disp(false);
    break;
  case 'b':
    // input = 1;
    // setInput();
    // main_disp(false);
    break;
  case 'c':
    // input = 2;
    // setInput();
    // main_disp(false);
    break;
  case 'd':
    // IIR = 0;
    // setIIR();
    // main_disp(false);
    break;
  case 'e':
    // IIR = 1;
    // setIIR();
    // main_disp(false);
    break;
  case 'f':
    // IIR = 2;
    // setIIR();
    // main_disp(false);
    break;
  case 'g':
    // IIR = 3;
    // setIIR();
    // main_disp(false);
    break;
  case 'h':
    // mute = 0;
    dac->unmute();
    // main_disp(false);
    break;
  case 'i':
    // mute = 1;
    dac->mute();
    // main_disp(false);
    break;
  case 'j':
    // getLockStatus();
    // getSR();
    // main_disp(false);
    break;
  case '0':
    // filter = 0;
    dac->setFilter(0);
    // main_disp(false);
    break;
  case '1':
    // filter = 1;
    dac->setFilter(1);
    // main_disp(false);
    break;
  case '2':
    // filter = 2;
    dac->setFilter(2);
    // main_disp(false);
    break;
  case '3':
    // filter = 3;
    dac->setFilter(3);
    // main_disp(false);
    break;
  case '4':
    // filter = 4;
    dac->setFilter(4);
    // main_disp(false);
    break;
  case '5':
    // filter = 5;
    dac->setFilter(5);
    // main_disp(false);
    break;
  case '6':
    // filter = 6;
    dac->setFilter(6);
    // main_disp(false);
    break;
  case 'y':
    dac->PrintDebugReg(dac->getDacAddress());
    break;
  case 'z':
    displayHelp();
    break;
  case '!':
    // initEEPROM();
    break;
  case '@':
    // DeviceInit();
    break;
  case '^':
    // testEEPROM();
    break;
  case 'p':
    dac->PrintDebugRegAll(dac->getDacAddress());
    break;
  default:
    break;
  }
}

// ----------------------------------------------- Beginning of setup()---------------------------------------------------------------
void setup()
{

  Serial.begin(115200); // for debugging
  
  dac = new DAC();
  daci2c = new I2C();

  Serial.println(F("DAC set up"));

  // Set up MCP23008 pins
  // mcp.begin(); // use default address 0
  // mcp.pinMode(0, OUTPUT);
  // mcp.pinMode(1, OUTPUT);
  // mcp.pinMode(2, OUTPUT);
  // mcp.pinMode(3, OUTPUT);
  // mcp.pinMode(4, OUTPUT);
  // mcp.pinMode(5, OUTPUT);
  // mcp.pinMode(6, OUTPUT);
  // mcp.pinMode(7, OUTPUT);

  // irrecv.enableIRIn(); // Start the IR receiver
  // Serial.println(F("IR on"));
}
void loop()
{
  // put your main code here, to run repeatedly:
  processCommand();
}