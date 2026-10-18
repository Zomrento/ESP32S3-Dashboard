# ESP32S3-Dashboard (EPD)

This project was meant as a way to experiment with C/C++ and 
my recently acquired [ESP32S3 from Seed Studio](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html)  

In the end the project turned out as a wallmounted [E-Paperdisplay](https://www.seeedstudio.com/4-26-Monochrome-SPI-ePaper-Display-p-6398.html)  
that periodically changes from widget to widget and displays things  
like a todo-list, the time, IT jokes, or silly comments from an LLM. 

The following will provide a brief overview over the two main areas 

## Structure

The project is divided into two parts:  
**Widgets** and **Communication**

### Widgets

Widgets are drawables for the display with varying layout
and functions. The project is build around the idea of 
having multiple widgets with different functionality and then
displaying them in turn. 

Excerpt from widgets.h:

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

### Communication

The ESP can communicate via HTTP/HTTPS.

For the LLM Quote Widget, I set up a python script and
an LLM running on Ollama on a private server as system-daemon
to be called as API. The ESP will reach out a few minutes in advance 
to the server and ask for a new comment for the LLM-widget. This comment
is called quote internally. Once the EPD switches to the LLM-widget it
will reach out again to collect the generated quote. 

> **NOTE**
> A WiFi-Connection is necessary for communication.
> To configure Network-Settings and Access-Control to API
> you need to set up a  Secrets.cpp, there is an ExampleSecrets.cpp for reference
> (Python-API and server need to be setup yourself)

    // Replace the placeholders with the actual values from the local network 
    // and server API you want to connect to and uncomment the code
    // After that save the changes and rename the file to "Secrets.cpp" (without the quotation marks)
    
    /*
    #include "Secrets.h"
    const char* ssid = "SSID";
    const char* password = "PASSWORD";
    const char* HTTP_AUTH_NAME = "SERVER_USER";
    const char* HTTP_AUTH_PWD = "SERVER_PASSWORD";
    const char* MY_SERVER_API = "https://google.com/api";
    */


Aside from this outgoing communication, the EPD can also be 
addressed inside your local network via HTTP by typing

>     http://ESPs_Local_IP

that way you can access a simple DEBUG GUI for quick configuration
there you will find a few quick commands as fill-ins, but I recommend
to read the shrimp.h and shrimp.cpp and then use the FREESTYLE option
to address all functions of the EPD by building shrimp commands directly.

![The Shrimp GUI](/include/img/image.png "Shrimp GUI")

## The Shrimp Protocol

The Shrimp protocol is a small protocol I spontaneously
came up with during this project for communication over HTTP/HTTPS
it is meant as a way to send small commands to and from the EPD.

It´s Syntax is quite simply: **CMDLENGTH** **CMDBYTE** **DATA**

Where **CMDLENGTH** holds the length of the whole command minus the first
Byte itself (ergo number of Bytes in command -1) this is due to how i handled decoding.
Note: since this value has a max of 255, shrimp-cmds and the **DATA** accompanying them
should be kept short.

**CMDBYTE**, sometimes also called **CMDID** holds the identifier for the command.
For example: 0x11 is a command that **SETS** the the data from the **DATA** section
as a TODO in the Todo-widget at the index that is specified in the first Byte from 
the **DATA** section. 

**DATA** holds additional values, be just one or to additional short integers, 
text or a mix of the two, the way **DATA** is interpreted depends on the associated command.
