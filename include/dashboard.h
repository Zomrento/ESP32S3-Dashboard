#include "TFT_eSPI.h"

void setFont(const GFXfont* &font, EPaper &epaper);
void setTime (char newhour [3], char newminute [3]);
void drawPartial(EPaper &epaper, String text, int x, int y);
void drawTime (EPaper &epaper);
void updateTime (EPaper &epaper);
