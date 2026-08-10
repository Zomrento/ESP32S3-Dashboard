#include "widgets.h"
#include "dashboard.h"
#include "fonts.h"
#include "img.h"


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
    if(quote.isEmpty()){
    /*  epaper.drawBitmap(0,40, luomiMain, 400, 400, TFT_BLACK);
        epaper.update();
        for(int i = 10; i > 0; i--){
            if(i % 2 == 0){
                epaper.fillRect(170,220,39,20, TFT_WHITE);
                epaper.drawBitmap(170, 220, luomiMouthOpen, 39, 20, TFT_BLACK);
            } else{
                epaper.fillRect(170,220,39,20, TFT_WHITE);
                epaper.drawBitmap(170, 220, luomiMouthClosed, 39, 20, TFT_BLACK);
            }
            epaper.updataPartial(170, 220, 39, 20);
            delay(1000);
        }
    */
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
    epaper.setFreeFont(&InterTight_VariableFont_wght32pt7b);
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

DEBUGWidget::DEBUGWidget(){
    id = -1;
    type = DEBUG;
    msg = "NULL";
    showError = false;
}

DEBUGWidget::DEBUGWidget(int8_t _id){
    id = _id;
    type = DEBUG;
    msg = "NULL";
    showError = false;
}

void DEBUGWidget::setMsg(String _msg){
    msg = _msg;
}

void DEBUGWidget::drawError(EPaper &epaper){
    epaper.drawString(error, epaper.width()/2, epaper.height()/2);
}

void DEBUGWidget::drawMsg(EPaper &epaper){
    epaper.drawString("Status:" + status, epaper.width()/2, epaper.height()/4);
    epaper.drawString("Content-Type:" + typemsg, epaper.width()/2, epaper.height()/2);
    epaper.drawString(msg, epaper.width()/2, epaper.height()/2+ epaper.height()/4);
}

void DEBUGWidget::drawWidget(EPaper &epaper){
    int8_t prev = epaper.getTextDatum();
    epaper.setTextDatum(MC_DATUM);
    epaper.setFreeFont(&InterTight_VariableFont_wght24pt7b);
    epaper.fillScreen(TFT_WHITE);
    if(showError){
        drawError(epaper);
    }
    else{drawMsg(epaper);
    }
    epaper.setTextDatum(prev);
    drawTime(epaper);
    epaper.update();
}

void DEBUGWidget::update(){
        HTTPResult result = getHTTPResult();
        error = result.x_error;
        status = result.statuscode;
        typemsg = result.contentType;
        msg = result.content;
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
        case DEBUG:
            break;
        default:
            current = &startupwidget;
    }
    current->drawWidget(epaper);
}

void WidgetMaster::drawCurrent(EPaper &epaper){
    current->drawWidget(epaper);
}

WidgetMaster::WidgetMaster(){
    startupwidget = StartUpWidget(0);
    luomiwidget = LuomiWidget();
    luomiwidget.id = 1;
    todowidget = TodoWidget();
    todowidget.id = 2;
    debugwidget = DEBUGWidget();
    debugwidget.id = 3;
    current = &debugwidget;
}