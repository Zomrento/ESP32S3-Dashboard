#include "TFT_eSPI.h"
#include "fonts.h"
#include "dashboard.h"

unsigned int shrimpCMD(uint8_t cmdArray[255], EPaper &epaper){
    uint8_t cmdLength = cmdArray[0];
    uint8_t cmd = cmdArray[1];
    String text = "";

    switch (cmd){
      // Help command
      case 0x00:
        return 0;
      // SetFont command
      case 0x01:
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
      // DrawText command
      case 0x02:
      {
        text = "";
        int x = cmdArray[2];
        int y = cmdArray[3];
        for(int i = 4; i < cmdLength + 1; i++){
          text += (char)cmdArray[i];
        }
        drawPartial(epaper, text, x, y);
        return 1;
      }
      // SetTime command
      case 0x03:
      {
        char new_hour[3] = {(char)cmdArray[2], (char)cmdArray[3], '\0'};
        char new_min[3] = {(char)cmdArray[4], (char)cmdArray[5], '\0'};
        setTime(new_hour, new_min);
        return 1;
      }
      default:
        return -1;
    }
}