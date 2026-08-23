#include "dashboard.h"
#include "fonts.h"
#include "shrimp.h"

char timeString [6] = "XX:XX";
unsigned long lastTime = 0;
char hourString [3] = "00";
char minuteString [3] = "00";
uint8_t cycInterval = 5;
uint8_t cycCountDown = cycInterval;
int8_t responseCountDown = -1;
byte autoGetRespCMD[2] = {0x01, 0x09};

/// SETTER

void setFont(const GFXfont* &font, EPaper &epaper){epaper.setFreeFont(font);}

void setCycle(uint8_t _minCountdown){cycCountDown = _minCountdown;}

void setResponseCountDown(int8_t _countdown){responseCountDown = _countdown;}

void setCycleInterval(uint8_t _minInterval){
    cycInterval = _minInterval;
    setCycle(cycInterval);
}

void setTime (char newhour [3], char newminute [3]){
    if(atoi(newhour)< 25){
        strcpy(hourString, newhour);
    }
    if(atoi(newminute)< 61){
        strcpy(minuteString, newminute);
    }
}

String getTimeString(){return timeString;}

void resetCycle(){cycCountDown = cycInterval;}


void drawPartial(EPaper &epaper, String text, unsigned int x, unsigned int y){
    epaper.setTextDatum(TL_DATUM);
    epaper.drawString(text, x, y);
    epaper.updataPartial(x, y, epaper.textWidth(text), epaper.fontHeight());
}

/// @brief 
/// @param epaper 
/// @param partialUpdate 
void drawTime(EPaper &epaper, bool partialUpdate){
    uint8_t prevDatum = epaper.getTextDatum();
    epaper.setTextDatum(TR_DATUM);
    epaper.setFreeFont(&InterTight_VariableFont_wght12pt7b);
    // To account for non MonoSpace Fonts default textwidth to account for is set to "XX:XX"
    epaper.fillRect(epaper.width() - 20 - epaper.textWidth("XX:XX"), 0, epaper.textWidth("XX:XX"), epaper.fontHeight(), TFT_WHITE);
    if(partialUpdate){
        epaper.fillRect(epaper.width() - 20 - epaper.textWidth("XX:XX"), 0, epaper.textWidth("XX:XX"), epaper.fontHeight(), TFT_WHITE);
        epaper.updataPartial(epaper.width() - 20 - epaper.textWidth("XX:XX"), 0, epaper.textWidth("XX:XX"), epaper.fontHeight());
        epaper.drawString(timeString, epaper.width() - 20, 0);
        epaper.updataPartial(epaper.width() - 20 - epaper.textWidth("XX:XX"), 0, epaper.textWidth("XX:XX"), epaper.fontHeight());
    };
    epaper.drawString(timeString, epaper.width() - 20, 0);
    epaper.setTextDatum(prevDatum);
}


void updateTime (EPaper &epaper, WidgetMaster &widgetMaster){
    if (millis() -lastTime >= 60000){
        lastTime += 60000;
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
        if(responseCountDown > 0){
            responseCountDown--;
        }
        else if (responseCountDown == 0)
        {
            responseCountDown = -1;
            shrimpCMD(autoGetRespCMD, epaper, widgetMaster);
        }
        cycCountDown--;
        if(cycCountDown == 0){
            resetCycle();
            widgetMaster.cycleWidget(epaper);
        }
        else{
            if(widgetMaster.current == &widgetMaster.timewidget){
                widgetMaster.current->drawWidget(epaper);
            }
            else{
                drawTime(epaper, true);
            }
        }  
    }
}

