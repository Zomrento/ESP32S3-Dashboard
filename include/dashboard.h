#pragma once
#include "TFT_eSPI.h"
#include "widgets.h"

/// @brief sets textFont to the specified font
/// @param font Pointer to a FreeFont installed in fonts.h 
/// @param epaper the epaper object initialized in main.cpp
void setFont(const GFXfont* &font, EPaper &epaper);

/// @brief sets absolute time to specified valie
/// @param newhour hour to set to 
/// @param newminute minute to set to
void setTime (char newhour [3], char newminute [3]);

/// @brief draws a String and partially updates used space
/// @param epaper reference to the used epaper-display
/// @param text String to draw
/// @param x x-coordinate to draw to
/// @param y y-coordinate to draw to
/// @note for epaper always use the initialized epaper from main.cpp
void drawPartial(EPaper &epaper, String text, unsigned int x, unsigned int y);

/// @brief draws the current time on screen
/// @param epaper reference to the used epaper-display
/// @param partialUpdate optionally refreshes the used displaypart
/// @note for epaper always use the initialized epaper from main.cpp
void drawTime(EPaper &epaper, bool partialUpdate = false);


/// @brief checks/updates time, then draws it and updates the screen
/// @param epaper reference to the used epaper-display
/// @note for epaper always use the initialized epaper from main.cpp
void updateTime (EPaper &epaper, WidgetMaster &widgetMaster);

/// @brief sets time until next widgetmaster.cycle
/// @param _minCountdown time to set cycleCountdown to
void setCycle(uint8_t _minCountdown);

void resetCycle();

/// @brief sets default time until next widgetmaster.cycle
/// @param _minInterval time to set cycleCountdown to
void setCycleInterval(uint8_t _minInterval);

/// @brief sets time until querying python-server for shrimpCMD(0x01, 0x09) FETCH RESPONSE
/// @param _countdown time to set to
void setResponseCountDown(int8_t _countdown);
