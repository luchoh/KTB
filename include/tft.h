#include <UTFT.h>              // Library for the TFT
#include <UTFT_DLB.h>          // Extension for the UTFT library


#ifdef BIG
UTFT_DLB myGLCD(ITDB43, 38, 39, 40, 41);
int tft_x = 480;
int tft_y = 272;
#else
UTFT_DLB myGLCD(ILI9341_16, 38, 39, 40, 41);
// UTFT_DLB myGLCD(ITDB32WD, 38, 39, 40, 41);
int tft_x = 400;
int tft_y = 240;
#endif

class TFT
{

public:
    TFT();

    void setup();
};