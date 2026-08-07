#include "widgets.h"
#include "dashboard.h"
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
    drawTime(epaper);
    epaper.update();
}

LuomiWidget::LuomiWidget(){
    id = -1;
    type = LUOMI_QUOTE; 
    quote = "";
}

LuomiWidget::LuomiWidget(int8_t _id, String _quote){
    id = _id;
    type = LUOMI_QUOTE;
    quote = _quote;
}

void StartUpWidget::drawWidget(EPaper &epaper){
    int8_t prev = epaper.getTextDatum();
    epaper.setTextDatum(MC_DATUM);
    epaper.fillScreen(TFT_WHITE);
    epaper.drawString("STARTUP", epaper.width()/2, epaper.height()/2);
    epaper.setTextDatum(prev);
    drawTime(epaper);
    epaper.update();
}

StartUpWidget::StartUpWidget(){
    id = -1;
    type = STARTUP;
}

StartUpWidget::StartUpWidget(int8_t _id){
    id = _id;
    type = STARTUP;
}

void WidgetMaster::cycleWidget(EPaper &epaper){
    switch(current->type){
        case STARTUP:
            current = &luomiwidget;
            break;
        case LUOMI_QUOTE:
            current = &todowidget;
            break;
        case TODO_LIST:
            current = &luomiwidget;
            break;
        default:
            current = &startupwidget;
    }
    current->drawWidget(epaper);
}

WidgetMaster::WidgetMaster(){
    startupwidget = StartUpWidget(0);
    luomiwidget = LuomiWidget();
    luomiwidget.id = 1;
    todowidget = TodoWidget();
    todowidget.id = 2;
    current = &startupwidget;
}