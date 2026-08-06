#include "TFT_eSPI.h"

enum WidgetType{
    IMAGE,
    TODO_LIST,
    LUOMI_QUOTE,
    ANIMATION
};


struct Widget {
    int8_t id;
    WidgetType type;
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
    LuomiWidget(int8_t _id, String _quote, EPaper &epaper);
};