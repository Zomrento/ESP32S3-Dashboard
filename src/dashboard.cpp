#include "dashboard.h"
#include "fonts.h"

char timeString [6] = "XX:XX";
unsigned long lastTime = 0;
char hourString [3] = "00";
char minuteString [3] = "00";
uint8_t minCountDown = 10;

void setFont(const GFXfont* &font, EPaper &epaper){
    epaper.setFreeFont(font);
}


void setTime (char newhour [3], char newminute [3]){
    strcpy(hourString, newhour);
    strcpy(minuteString, newminute);
}

/// @brief draws a String and partially updates used space
/// @param epaper reference to the used epaper-display
/// @param text String to draw
/// @param x x-coordinate to draw to
/// @param y y-coordinate to draw to
/// @note for epaper always use the initialized epaper from main.cpp
void drawPartial(EPaper &epaper, String text, unsigned int x, unsigned int y){
    epaper.setTextDatum(TL_DATUM);
    epaper.drawString(text, x, y);
    epaper.updataPartial(x, y, epaper.textWidth(text), epaper.fontHeight());
}

/// @brief draws the current time on screen
/// @param epaper reference to the used epaper-display
/// @param partialUpdate optionally refreshes the used displaypart
/// @note for epaper always use the initialized epaper from main.cpp
void drawTime(EPaper &epaper, bool partialUpdate){
    uint8_t prevDatum = epaper.getTextDatum();
    epaper.setTextDatum(TR_DATUM);
    epaper.setFreeFont(&InterTight_VariableFont_wght12pt7b);
    epaper.drawString(timeString, epaper.width() - 40, 40);
    epaper.setTextDatum(prevDatum);
    if(partialUpdate){
        epaper.updataPartial(epaper.width() - 40, 40, epaper.textWidth(timeString), epaper.fontHeight());
    };
}

/// @brief checks/updates time, then draws it and updates the screen
/// @param epaper reference to the used epaper-display
/// @note for epaper always use the initialized epaper from main.cpp
void updateTime (EPaper &epaper, WidgetMaster &widgetMaster){
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
        minCountDown--;
        if(minCountDown == 0){
            minCountDown = 10;
            widgetMaster.cycleWidget(epaper);
        }
        else{
            drawTime(epaper, true);
        }
        
    }
}

