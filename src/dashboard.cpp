#include "dashboard.h"
#include "TFT_eSPI.h"
#include "fonts.h"
//test

char timeString [6] = "XX:XX";
unsigned long lastTime = 0;
char hourString [3] = "00";
char minuteString [3] = "00";

void setFont(const GFXfont* &font, EPaper &epaper){
    epaper.setFreeFont(font);
}


void setTime (char newhour [3], char newminute [3]){
    strcpy(hourString, newhour);
    strcpy(minuteString, newminute);
}

void drawPartial(EPaper &epaper, String text, int x, int y){
    epaper.setTextDatum(TL_DATUM);
    epaper.drawString(text, x, y);
    epaper.updataPartial(x, y, epaper.textWidth(text), epaper.fontHeight());
}

void drawTime(EPaper &epaper){
    epaper.setTextDatum(TR_DATUM);
    epaper.setFreeFont(&InterTight_VariableFont_wght24pt7b);
    epaper.drawString(timeString, epaper.width() - 40, 40);
    epaper.updataPartial(epaper.width() - 40, 40, epaper.textWidth(timeString), epaper.fontHeight());
}

void updateTime (EPaper &epaper){
    if (millis() -lastTime >= 60000){
        lastTime = millis();
        if(atoi(minuteString) != 59){
            snprintf(minuteString, sizeof(minuteString), "%02d", atoi(minuteString) + 1);    
        } else{
            strcpy(minuteString, "00");
            if(atoi(hourString) != 23){
                snprintf(hourString, sizeof(hourString), "%02d", atoi(hourString) + 1);    
            } else{
                strcpy(hourString, "00");
            }
        }
        snprintf(timeString, sizeof(timeString), "%s:%s", hourString, minuteString);
        drawTime(epaper);
    }
}

