#pragma once
#include "TFT_eSPI.h"

enum WidgetType{
    STARTUP,
    IMAGE,
    TODO_LIST,
    LUOMI_QUOTE,
    ANIMATION
};

struct Widget {
    int8_t id;
    WidgetType type;
    virtual void drawWidget(EPaper &epaper) = 0;
};

struct TodoWidget : Widget {
    uint8_t taskCount;
    String todolist[8];
    void drawWidget(EPaper &epaper);
    TodoWidget();
    TodoWidget(int8_t _id, uint8_t _taskCount, String _todolist[8], EPaper &epaper);
};

struct LuomiWidget : Widget {
    String quote;
    void drawWidget(EPaper &epaper);
    LuomiWidget();
    LuomiWidget(int8_t _id, String _quote);
};

struct StartUpWidget : Widget {
    void drawWidget(EPaper &epaper);
    StartUpWidget();
    StartUpWidget(int8_t _id);
};

struct WidgetMaster {
    Widget* current;
    StartUpWidget startupwidget;
    TodoWidget todowidget;
    LuomiWidget luomiwidget;
    void cycleWidget(EPaper &epaper);
    WidgetMaster();
};
