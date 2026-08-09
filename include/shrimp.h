#pragma once
#include "TFT_eSPI.h"
#include "widgets.h"


int8_t shrimpCMD(uint8_t cmdArray[255], EPaper &epaper, WidgetMaster &widgetMaster);