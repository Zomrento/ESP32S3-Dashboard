#include "TFT_eSPI.h"
#include "dashboard.h"

enum WidgetType{
    IMAGE,
    TODO_LIST,
    LUOMI_QUOTE,
    ANIMATION
};

struct Widget{
    unsigned int id;
    WidgetType type;
};

struct TodoWidget : Widget {
    uint8_t taskCount;
    String todolist[8];
    void drawWidget(EPaper &epaper, int x, int y){
        epaper.fillScreen(TFT_WHITE);
        epaper.drawString("Todo-List", 200, 0);
            for (uint8_t i = 0; i < taskCount; i++){
                epaper.drawString(String(i) + ": " + todolist[i],50, i*50+75);
                epaper.drawLine(50, epaper.fontHeight()+(i*50)+70, 350, epaper.fontHeight()+(i*50)+70, TFT_BLACK);
            }
        drawTime(epaper);
        epaper.update();
        }

    TodoWidget(unsigned int _id, uint8_t _taskCount, String _todolist[8], EPaper &epaper){
        id = _id;
        taskCount = _taskCount;
        type = TODO_LIST;
        if  (taskCount < 9){
            epaper.drawString("Error: widgets.cpp, taskcount out of range", 150, 200);
            epaper.update();
        };
        for(uint8_t i = 0; i < taskCount; i++){
            todolist[i] = _todolist[i];
        } 
    };
};

struct LuomiWidget : Widget{

};