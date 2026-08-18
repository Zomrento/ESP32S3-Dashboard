#include "widgets.h"
#include "dashboard.h"
#include "fonts.h"
#include "img.h"
#include "WiFi.h"
#include "shrimp.h"


void TodoWidget::drawWidget(EPaper &epaper){
    epaper.fillScreen(TFT_WHITE);
    uint8_t prev = epaper.getTextDatum();
    epaper.setTextDatum(TL_DATUM);
    epaper.setFreeFont(&InterTight_VariableFont_wght18pt7b);
    epaper.drawString("Todo-List", 200, 0);
        for (uint8_t i = 0; i < 8; i++){
            if (!todolist[i].isEmpty()){
                epaper.drawString(String(i) + ": " + todolist[i],50, i*50+75);
                epaper.drawLine(50, epaper.fontHeight()+(i*50)+70, 350, epaper.fontHeight()+(i*50)+70, TFT_BLACK);
            }
        }
    epaper.setTextDatum(prev);
    drawTime(epaper);
    epaper.update();
}

bool TodoWidget::addTask(String _tasks[]){
    for(uint8_t i = 0; i < sizeof(_tasks); i++){
        for (uint8_t j = i; j < 8; j++){
            if(todolist[j].isEmpty()){
                todolist[j] = _tasks[i];
                break;
            }
            // if the inner loop goes to the last index and finds it occupied return false
            else if (j == 7){
                return false;
            }
        }
    }
    // if everythings fine return true
    return true;
}

bool TodoWidget::addTask(String _task){
    for (uint8_t i = 0; i < 8; i++){
        if(todolist[i].isEmpty()){
            todolist[i] = _task;
            break;
        }
        // loop goes to the last index and finds it occupied return false
        else if (i == 7){
            return false;
        }
    }
    // if everythings fine return true
    return true;
}

void TodoWidget::setTask(uint8_t index, String _task){
    if(index < 8){
        todolist[index] = _task;
    }
}

void TodoWidget::removeLast(uint8_t num){
    int8_t i = 7;
    while(i != -1 && num !=0){
        if(!todolist[i].isEmpty()){
            todolist[i] = "";
            num--;
        }
        i--;
    }
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

void QuoteWidget::update(u8_t _model, u8_t _type, String _quote){
    model = _model;
    quoteType = _type;
    quote = _quote;
}

void QuoteWidget::drawWidget(EPaper &epaper){
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
        if (model==0x02){
            epaper.drawString("Nyx:", 50, 0);
            epaper.drawString(quote, 80, epaper.fontHeight()+5);
            // Depending on which quoteType is associated with the quote, a diffrent image, 
            // appropiately for the quotetype needs to be displayed

            switch(quoteType){
                // Default
                case 0x00:
                {
                    epaper.drawBitmap(epaper.width()-350, epaper.height()-350, nyxtalk, 350, 350, TFT_BLACK);
                    break;
                }
                // Sarcastic
                case 0x01:
                {
                    epaper.drawBitmap(epaper.width()-350, epaper.height()-350, nyxsmoking, 350, 350, TFT_BLACK);
                    break;
                }
                // Tired
                case 0x02:
                {
                    epaper.drawBitmap(epaper.width()-350, epaper.height()-350, nyxsleep, 350, 350, TFT_BLACK);
                    break;
                }
                // Menace
                case 0x03:
                {
                    epaper.drawBitmap(epaper.width()-350, epaper.height()-350, nyxPissed, 350, 350, TFT_BLACK);
                    break;
                }
                // Mischievous
                case 0x04:
                {
                    epaper.drawBitmap(epaper.width()-350, epaper.height()-350, nyxmischievous, 350, 350, TFT_BLACK);
                    break;
                }


            }
        }
    }
    epaper.setTextDatum(prev);
    drawTime(epaper);
    epaper.update();
}

QuoteWidget::QuoteWidget(){
    id = -1;
    type = QUOTE; 
    quote = "";
    model = 0x01;
    quoteType = 0x00;
}

QuoteWidget::QuoteWidget(int8_t _id, String _quote, u8_t _model){
    id = _id;
    type = QUOTE;
    quote = _quote;
    model = _model;
    quoteType = 0x05;
}

void StartUpWidget::drawWidget(EPaper &epaper){
    int8_t prev = epaper.getTextDatum();
    epaper.setTextDatum(TC_DATUM);
    epaper.fillScreen(TFT_WHITE);
    epaper.setFreeFont(&BeauRivage_Regular32pt7b);
    epaper.drawString("ESP32S3 EPaper-Dashboard", epaper.width()/2, 0);
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


void JokeWidget::getJoke(){
    String rawData = sendHTTPRequest("http://official-joke-api.appspot.com/jokes/programming/random");
    setup = rawData.substring(rawData.indexOf("\"setup\":\"") + 9, rawData.indexOf("\"punchline\": \"")-2);
    punchline = rawData.substring(rawData.indexOf("\"punchline\":\"") + 13, rawData.indexOf("\"id\":")-2);
}

void JokeWidget::drawWidget(EPaper &epaper){
    int8_t prev = epaper.getTextDatum();
    epaper.setTextDatum(TC_DATUM);
    epaper.fillScreen(TFT_WHITE);
    epaper.setFreeFont(&BeauRivage_Regular32pt7b);
    epaper.drawString("JokeWidget", epaper.width()/2, 0);
    epaper.setFreeFont(&BeauRivage_Regular24pt7b);
    epaper.setTextDatum(TL_DATUM);
    epaper.drawString(setup, 50, 100);
    epaper.drawString(punchline, 50, 100 + epaper.fontHeight()+20);
    epaper.setTextDatum(prev);
    drawTime(epaper);
    epaper.update();

}

JokeWidget::JokeWidget(){
    id = -1;
    getJoke();
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
    epaper.setFreeFont(&InterTight_VariableFont_wght18pt7b);
    epaper.fillScreen(TFT_WHITE);
    if(showError){
        drawError(epaper);
    }
    else{drawMsg(epaper);
    }
    epaper.setTextDatum(TL_DATUM);
    epaper.drawString(WiFi.localIP().toString(), 0, 0);
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
    if(!cycleBlock){
        switch(current->type){
            case STARTUP:
            {
                uint8_t cmd[] = {0x01, 0x07};
                shrimpCMD(cmd, epaper, *this);
                current = &todowidget;
                break;
            }
            case TODO_LIST:
            {
                jokewidget.getJoke();
                current = &jokewidget;
                break;
            }
            case JOKE:{
                current = &quotewidget;
                break;
            }
            case QUOTE:
            {
                uint8_t cmd[] = {0x01, 0x07};
                shrimpCMD(cmd, epaper, *this);
                current = &todowidget;
                break;
            }
            case DEBUG:{
                break;
            }
            default:{
                current = &startupwidget;
            }
        }   
        current->drawWidget(epaper);
    }
}

void WidgetMaster::drawCurrent(EPaper &epaper){
    current->drawWidget(epaper);
}

void WidgetMaster::setCycleBlock(EPaper &epaper, bool setTo){
    cycleBlock = setTo;
    current->drawWidget(epaper);
}

WidgetMaster::WidgetMaster(){
    startupwidget = StartUpWidget(0);
    quotewidget = QuoteWidget();
    quotewidget.id = 1;
    todowidget = TodoWidget();
    todowidget.id = 2;
    debugwidget = DEBUGWidget();
    debugwidget.id = 3;
    current = &startupwidget;
    cycleBlock = false;
}