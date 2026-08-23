#include "widgets.h"
#include "dashboard.h"
#include "fonts.h"
#include "img.h"
#include "WiFi.h"
#include "shrimp.h"



void StartUpWidget::drawWidget(EPaper &epaper){
    int8_t prev = epaper.getTextDatum();
    epaper.setTextDatum(TC_DATUM);
    epaper.fillScreen(TFT_WHITE);
    epaper.setFreeFont(&PlayfairDisplay_VariableFont_wght32pt7b);
    epaper.drawString("ESP32S3 EPaper-Dashboard", epaper.width()/2, 50);
    epaper.setFreeFont(&BeauRivage_Regular24pt7b);
    epaper.setTextDatum(TL_DATUM);
    epaper.drawString("by Shrimpmoth", 0, 150);
    epaper.update();
    delay(3000);
    epaper.drawString("featuring:", 100, 200);
    epaper.updataPartial(100, 200, epaper.textWidth("featuring: "), epaper.fontHeight());
    delay(3000);
    epaper.drawString("Luomi", 100 + epaper.textWidth("featuring: ") , 200);
    epaper.drawBitmap(epaper.width()-350, epaper.height()-350, luomiTitle, 350, 350, TFT_BLACK);
    epaper.updataPartial(100+epaper.textWidth("featuring: "), 200, epaper.textWidth("Luomi"), epaper.fontHeight());
    epaper.updataPartial(epaper.width()-350, epaper.height()-350, 350, 350);
    epaper.setTextDatum(prev);
    drawTime(epaper, true);
    
}

StartUpWidget::StartUpWidget(){type = STARTUP;}

void TodoWidget::drawWidget(EPaper &epaper){
    epaper.fillScreen(TFT_WHITE);
    uint8_t prev = epaper.getTextDatum();
    epaper.setTextDatum(TL_DATUM);
    epaper.setFreeFont(&InterTight_VariableFont_wght24pt7b);
    epaper.drawString("Todo-List", 200, 0);
    epaper.setFreeFont(&InterTight_VariableFont_wght18pt7b);
        for (uint8_t i = 0; i < 8; i++){
            if (!todolist[i].isEmpty()){
                Serial.println(todolist[i]);
                epaper.drawString((String(i) + ": " + String(todolist[i])),50, i*50+75);
                epaper.drawLine(50, epaper.fontHeight()+(i*50)+70, 350, epaper.fontHeight()+(i*50)+70, TFT_BLACK);
            }
        }
    epaper.setTextDatum(prev);
    drawTime(epaper);
    epaper.update();
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

TodoWidget::TodoWidget(){type = TODO_LIST;}

void QuoteWidget::drawWidget(EPaper &epaper){
    int8_t prev = epaper.getTextDatum();
    epaper.setTextDatum(TC_DATUM);
    epaper.fillScreen(TFT_WHITE);
    epaper.setFreeFont(&InterTight_VariableFont_wght32pt7b);
    epaper.drawString("Quote", epaper.width()/2, 0);
    epaper.setTextDatum(TL_DATUM);
    if(quote.isEmpty()){
        epaper.drawString("WARN: NO QUOTE", epaper.width()/2, epaper.height()/2);
    } else{
        epaper.setFreeFont(&BeauRivage_Regular24pt7b);
        if (model==0x01){
            epaper.drawString("Luomi:", 50, 50);
            if(quote.length()>40){
                String quote1  = quote.substring(0, 40);
                String quote2 = quote.substring(40);
            }
            epaper.drawString(quote, 80, epaper.fontHeight()+55);
            // Depending on which quoteType is associated with the quote, a diffrent image, 
            // appropiately for the quotetype needs to be displayed
            switch(quoteType){
                // Default
                case 0x00:
                {
                    epaper.drawBitmap(epaper.width()-350, epaper.height()-350, luomiDefault, 350, 350, TFT_BLACK);
                    break;
                }
                // Sarcastic
                case 0x01:
                {
                    epaper.drawBitmap(epaper.width()-350, epaper.height()-350, luomiTalk, 350, 350, TFT_BLACK);
                    break;
                }
                // Tired
                case 0x02:
                {
                    epaper.drawBitmap(epaper.width()-350, epaper.height()-350, luomiTired, 350, 350, TFT_BLACK);
                    break;
                }
                // Menace
                case 0x03:
                {
                    epaper.drawBitmap(epaper.width()-350, epaper.height()-350, luomiAngry, 350, 350, TFT_BLACK);
                    break;
                }
                // Mischievous
                case 0x04:
                {
                    epaper.drawBitmap(epaper.width()-350, epaper.height()-350, luomiSmug, 350, 350, TFT_BLACK);
                    break;
                }
            }
        }
        else if (model==0x02){
            epaper.drawString("Nyx:", 50, 50);
            epaper.drawString(quote, 80, epaper.fontHeight()+55);
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

void QuoteWidget::update(u8_t _model, u8_t _type, String _quote){
    model = _model;
    quoteType = _type;
    quote = _quote;
}

QuoteWidget::QuoteWidget(){
    type = QUOTE; 
    quote = "INITQUOTE";
    model = 0x01;
    quoteType = 0x00;
}

QuoteWidget::QuoteWidget( String _quote, u8_t _model){
    type = QUOTE;
    quote = _quote;
    model = _model;
    quoteType = 0x05;
}

void JokeWidget::drawWidget(EPaper &epaper){
    int8_t prev = epaper.getTextDatum();
    epaper.setTextDatum(TC_DATUM);
    epaper.fillScreen(TFT_WHITE);
    epaper.setFreeFont(&BeauRivage_Regular32pt7b);
    epaper.drawString("JokeWidget", epaper.width()/2, 0);
    epaper.setFreeFont(&BeauRivage_Regular24pt7b);
    epaper.setTextDatum(TC_DATUM);
    epaper.drawString(setup, epaper.width()/2, 140);
    epaper.drawString(punchline, epaper.width()/2, 140 + epaper.fontHeight()+20);
    epaper.setTextDatum(prev);
    drawTime(epaper);
    epaper.update();

}

void JokeWidget::getJoke(){
    String rawData = sendHTTPRequest("http://official-joke-api.appspot.com/jokes/programming/random");
    setup = rawData.substring(rawData.indexOf("\"setup\":\"") + 9, rawData.indexOf("\"punchline\":\"")-2);
    punchline = rawData.substring(rawData.indexOf("\"punchline\":\"") + 13, rawData.indexOf("\"id\":")-2);
}

JokeWidget::JokeWidget(){type = JOKE;}

void TimeWidget::drawWidget(EPaper &epaper){
    uint8_t prevDatum = epaper.getTextDatum();
    epaper.fillScreen(TFT_WHITE);
    epaper.setTextDatum(MC_DATUM);
    epaper.setFreeFont(&PlayfairDisplay_VariableFont_wght32pt7b);
    epaper.setTextSize(2);
    epaper.drawString(getTimeString(), epaper.width()/2, epaper.height()/2);
    epaper.setTextDatum(prevDatum);
    epaper.setTextSize(1);
    epaper.update();
}

TimeWidget::TimeWidget(){type = TIME;}


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

/// DEBUGWIDGET
/// @brief Do not build any code dependand on DEBUGWidget as its main purpose is
///        Debugdiagnostics and it will be rewritten as seen fit on the fly

DEBUGWidget::DEBUGWidget(){
    type = DEBUG;
    msg = "NULL";
    showError = false;
}

void DEBUGWidget::setMsg(String _msg){msg = _msg;}

void DEBUGWidget::drawError(EPaper &epaper){epaper.drawString(error, epaper.width()/2, epaper.height()/2);}

void DEBUGWidget::drawMsg(EPaper &epaper){
    epaper.drawString(sendHTTPRequest("http://official-joke-api.appspot.com/jokes/programming/random"), epaper.width()/2, epaper.height()/4);
    epaper.update();
}

void DEBUGWidget::update(){
        HTTPSResult result = getHTTPSResult();
        error = result.x_error;
        status = result.statuscode;
        typemsg = result.contentType;
        msg = result.content;
}

void WidgetMaster::cycleWidget(EPaper &epaper){
    byte promptCmd[2] = {0x01, 0x07};
    if(!cycleBlock){
        switch(current->type){
            case STARTUP:
            {
                shrimpCMD(promptCmd, epaper, *this);
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
                shrimpCMD(promptCmd, epaper, *this);
                current = &timewidget;
                break;
            }
            case TIME:{
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

void WidgetMaster::drawCurrent(EPaper &epaper){current->drawWidget(epaper);}

void WidgetMaster::setCycleBlock(EPaper &epaper, bool _cycleBlock){
    cycleBlock = _cycleBlock;
    current->drawWidget(epaper);
}

WidgetMaster::WidgetMaster(){
    current = &startupwidget;
    cycleBlock = false;
}