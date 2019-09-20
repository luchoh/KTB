#include "tft.h"


TFT::TFT(){
    // Set up the TFT
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setBackColor(0, 0, 0);
}

void TFT::setup() {
    
}

