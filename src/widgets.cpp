#include "TFT_eSPI.h"
#include "dashboard.h"
#include "widgets.h"
#include "fonts.h"



void TodoWidget::drawWidget(EPaper &epaper){
    epaper.fillScreen(TFT_WHITE);
    epaper.drawString("Todo-List", 200, 0);
        for (uint8_t i = 0; i < taskCount; i++){
            epaper.drawString(String(i) + ": " + todolist[i],50, i*50+75);
            epaper.drawLine(50, epaper.fontHeight()+(i*50)+70, 350, epaper.fontHeight()+(i*50)+70, TFT_BLACK);
        }
    drawTime(epaper);
    epaper.update();
}

TodoWidget::TodoWidget(){
    id = -1;
    taskCount = 0;
    type = TODO_LIST; 
}

TodoWidget::TodoWidget(int8_t _id, uint8_t _taskCount, String _todolist[8], EPaper &epaper){
    id = _id;
    taskCount = _taskCount;
    type = TODO_LIST;
    if  (taskCount > 8){
        epaper.drawString("Error: widgets.cpp, taskcount out of range", 150, 200);
        epaper.update();
    } else{
        for(uint8_t i = 0; i < taskCount; i++){
            todolist[i] = _todolist[i];
        }
    }
}

void LuomiWidget::drawWidget(EPaper &epaper){
    int8_t prev = epaper.getTextDatum();
    epaper.setTextDatum(TC_DATUM);
    epaper.fillScreen(TFT_WHITE);
    epaper.drawString("Luomi-Quote", epaper.width()/2, 0);
    if(sizeof(quote) < 1){
        epaper.setFreeFont(&InterTight_VariableFont_wght32pt7b);
        epaper.drawString("WARN: NO QUOTE", epaper.width()/2, epaper.height()/2);
    } else{
        epaper.setFreeFont(&BeauRivage_Regular24pt7b);
        epaper.drawString(quote, epaper.width()/2, epaper.height()/2);
    }
    epaper.setTextDatum(prev);
    epaper.update();
}

LuomiWidget::LuomiWidget(){
    id = -1;
    type = LUOMI_QUOTE; 
    quote = "";
}

LuomiWidget::LuomiWidget(int8_t _id, String _quote, EPaper &epaper){
    id = _id;
    type = LUOMI_QUOTE;
    quote = _quote;
}