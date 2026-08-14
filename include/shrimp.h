#pragma once
#include "TFT_eSPI.h"
#include "widgets.h"



String getText(uint8_t start);

/// @brief function to handle shrimp-protocol
/// @param cmdArray byte array containing CommandID, CommandLength, and Data
/// @param epaper reference to the used epaper-display
/// @return a int8_t representing the status of the commandexecution
/// @note for epaper always use the initialized epaper from main.cpp
int8_t shrimpCMD(uint8_t cmdArray[255], EPaper &epaper, WidgetMaster &widgetMaster);