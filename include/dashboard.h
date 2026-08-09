#pragma once
#include "TFT_eSPI.h"
#include "widgets.h"

void setFont(const GFXfont* &font, EPaper &epaper);
void setTime (char newhour [3], char newminute [3]);
void drawPartial(EPaper &epaper, String text, unsigned int x, unsigned int y);
void drawTime(EPaper &epaper, bool partialUpdate = false);
void updateTime (EPaper &epaper, WidgetMaster &widgetMaster);
void setCountdown(uint8_t _minCountdown);
void setCountdownInterval(uint8_t _minInterval);
