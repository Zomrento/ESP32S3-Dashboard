#pragma once
#include "TFT_eSPI.h"
#include "httpHandler.h"

/// @brief A type assosciated as member of Widget objects
enum WidgetType{
    STARTUP,
    TODO_LIST,
    QUOTE,
    JOKE,
    TIME,
    DEBUG
    
};

/// @brief An abstract struct, defining a drawable window. Its derivatives implement specifics
struct Widget {
    WidgetType type;

    /// @brief abstract function to be implemented ad hoc by all of the structs derivatives
    //         defines the way the widget is to be displayed and then does so  
    /// @param epaper EPaper object initialized in teh main.cpp
    virtual void drawWidget(EPaper &epaper) = 0;
};

/// @brief A Widget-Derivative focusing on displaying a todolist
struct TodoWidget : Widget {
    String todolist[8];
    void drawWidget(EPaper &epaper);
    bool addTask(String _task);
    void setTask(uint8_t index, String _task);
    void removeLast(uint8_t num);
    TodoWidget();
};

/// @brief A Widget-Derivative focusing on displaying responses from my personal AI, Luomi. Mostly sarcastic Comments
struct QuoteWidget : Widget {
    String quote;
    uint8_t quoteType;
    uint8_t model;
    void update(u8_t _model, u8_t _type, String _quote);
    void drawWidget(EPaper &epaper);
    QuoteWidget();
    QuoteWidget( String _quote, u8_t _model);
};

/// @brief A Widget-Derivative being displayed upon device start
struct StartUpWidget : Widget {
    void drawWidget(EPaper &epaper);
    StartUpWidget();
};

/// @brief A Widget-Derivative for displaying Jokes
struct JokeWidget : Widget {
    String setup;
    String punchline;
    void drawWidget(EPaper &epaper);
    void getJoke();
    JokeWidget();
};

/// @brief Widget that just displays the time
struct TimeWidget : Widget {
    void drawWidget(EPaper &epaper);
    TimeWidget();
};

/// @brief A Widget-Derivative for displaying useful debug information
struct DEBUGWidget : Widget {
    HTTPSResult result;
    String typemsg;
    String msg;
    String status;
    String error;
    bool showError;
    
    /// @brief updates internal values with the result of the latest HTTPS request
    void update();

    /// @brief Subfunction of drawWidget, drawing error-message of latest rcvd https response
    /// @param epaper 
    void drawError(EPaper &epaper);

    /// @brief Subfunction of drawWidget, drawing status, type, and content of latest rcvd https response
    /// @param epaper 
    void drawMsg(EPaper &epaper);

    /// @brief setter for internal msg, holding the content of a received https response
    /// @param _msg 
    void setMsg(String _msg);
    void drawWidget(EPaper &epaper);
    DEBUGWidget();
};

/// @brief a struct holding one of each Widget-Derivative to manage their display
struct WidgetMaster {
    Widget* current;
    bool cycleBlock;
    StartUpWidget startupwidget;
    TodoWidget todowidget;
    QuoteWidget quotewidget;
    TimeWidget timewidget;
    JokeWidget jokewidget;
    DEBUGWidget debugwidget;
    void setCycleBlock(EPaper &epaper, bool _cycleBlock);
    /// @brief cycles to the next widget
    /// @param epaper EPaper object initialized in main.cpp
    void cycleWidget(EPaper &epaper);

    /// @brief draws the currently set Widget onto the screen
    /// @param epaper EPaper object initialized in main.cpp
    void drawCurrent(EPaper &epaper);
    WidgetMaster();
};
