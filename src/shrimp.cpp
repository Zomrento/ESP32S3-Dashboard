#include "shrimp.h"
#include "fonts.h"
#include "dashboard.h"
#include "httpHandler.h"

uint8_t cmdArray[255];
uint8_t cmdLength;
uint8_t cmd;
String text;

String getText(uint8_t start){
  String txt = "";
  for(int i = start; i < cmdLength + 1; i++){
          txt += (char)cmdArray[i];
        }
  return txt;      
}


/*  Commands:
      0x00: Help                           0x01 0x00
      0x01: SetFont                        0x02 0x01 
      0x02: DrawString
      0x03: SetTime
    Statuscodes sent on Return:
      -1: At least one command not recognized
      0: Sent HTTP with commandlist of this specific project to Client (Help)
      1: OK, nothing to do
*/
int8_t shrimpCMD(uint8_t cmdArray[255], EPaper &epaper, WidgetMaster &widgetMaster){
    cmdLength = cmdArray[0];
    cmd = cmdArray[1];
    text = "";

    switch (cmd){
      // Help command
      case 0x00:
      {
        return 0;
      }
      
      //-------------------------------------------
      // TEXT COMMANDS
      //-------------------------------------------

      // SetFont command
      case 0x01:
      {
        switch (cmdArray[2]){
          // i = from 0 to incl. 3: Inter
          case 0:
          epaper.setFreeFont(&InterTight_VariableFont_wght12pt7b);
          break;
          case 1:
          epaper.setFreeFont(&InterTight_VariableFont_wght18pt7b);
          break;
          case 2:
          epaper.setFreeFont(&InterTight_VariableFont_wght24pt7b);
          break;
          case 3:
          epaper.setFreeFont(&InterTight_VariableFont_wght32pt7b);
          break;
          // i = from 4 to incl. 6: Playfair Display
          case 4:
          epaper.setFreeFont(&PlayfairDisplay_VariableFont_wght18pt7b);
          break;
          case 5:
          epaper.setFreeFont(&PlayfairDisplay_VariableFont_wght24pt7b);
          break;
          case 6:
          epaper.setFreeFont(&PlayfairDisplay_VariableFont_wght32pt7b);
          break;
          // i = from 7 to incl. 8: BeauRivage
          case 7:
          epaper.setFreeFont(&BeauRivage_Regular24pt7b);
          break;
          case 8:
          epaper.setFreeFont(&BeauRivage_Regular32pt7b);
          break;
          // default is the same as i=2, yet i=2 still exist for uniformity purposes
          default:
          epaper.setFreeFont(&InterTight_VariableFont_wght24pt7b);
          break;
        }
        return 1;
      }
      // DrawText command
      case 0x02:
      {
        int x = cmdArray[2];
        int y = cmdArray[3];
        text = getText(4);
        drawPartial(epaper, text, x, y);
        return 1;
      }
      
      //-------------------------------------------
      // TIME COMMANDS
      //-------------------------------------------

      // SetTime command
      case 0x03:
      {
        char new_hour[3] = {(char)cmdArray[2], (char)cmdArray[3], '\0'};
        char new_min[3] = {(char)cmdArray[4], (char)cmdArray[5], '\0'};
        setTime(new_hour, new_min);
        return 1;
      }
      // Sets current Countdown to given number max 255
      case 0x04:
      {
        setCycle(cmdArray[2]);
        return 1;
      }
      // Sets Cycling Countdown Interval and set current timer to that time max 255
      case 0x05:
      {
        setCycleInterval(cmdArray[2]);
        return 1;
      }

      //-------------------------------------------
      // LUOMI COMMANDS
      //-------------------------------------------
      
      // Set Luomi Quote
      case 0x06:
      {
        text = getText(2);
        widgetMaster.luomiwidget.quote = text;
        return 1;
      }
      // Sends a query to python server for luomi quote with custom prompt
      ///@note currently modified for debug purposes
      case 0x07:
      {
        sendRequest(cmdArray,cmdLength+1);
        setResponseCountDown(5);
        // widgetMaster.debugwidget.update();
        // widgetMaster.current = &widgetMaster.debugwidget;
        // widgetMaster.drawCurrent(epaper);
        return 1;
      }
      // Sends a query to python server for luomi quote with standard prompt
      ///@note currently modified for debug purposes
      case 0x08:
      {
        sendRequest(cmdArray,cmdLength+1);
        setResponseCountDown(5);
        // widgetMaster.debugwidget.update();
        // widgetMaster.current = &widgetMaster.debugwidget;
        // widgetMaster.drawCurrent(epaper);
        return 1;
      }
      // Sends a query to python server to retrieve last generated quote
      ///@note currently modified for debug purposes
      case 0x09:
      {
        sendRequest(cmdArray,cmdLength+1);
        // widgetMaster.debugwidget.update();
        // widgetMaster.current = &widgetMaster.debugwidget;
        // widgetMaster.drawCurrent(epaper);
        return 1;
      }
      
      //-------------------------------------------
      // TODOLIST COMMANDS
      //-------------------------------------------

      // clears last X todolist entries
      case 0x10:
      {
        widgetMaster.todowidget.removeLast(cmdArray[2]);
        return 1;
      }
      // sets todolist index X to String
      case 0x11:
      {
        widgetMaster.todowidget.setTask(cmdArray[2], getText(3));
        return 1;
      }
      // adds todolist task
      case 0x12:
      {
        widgetMaster.todowidget.addTask(getText(2));
        return 1;
      }
      default:
        return -1;
    }
}