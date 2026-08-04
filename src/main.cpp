#include "driver.h"
#include "WiFi.h"
#include "TFT_eSPI.h"
#include "Secrets.h"
#include "fonts.h"
#include "dashboard.h"

// Set up EPaper object
EPaper epaper;
// Set web server port number to 80
WiFiServer server(80);
// Variable to store the HTTP request
String header;
String body;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
// Mockdata
const char* something = "Owo";

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  // Start up EPaper and flush the screen
  epaper.begin();
  epaper.setTextSize(2);
  epaper.fillScreen(TFT_WHITE);
  epaper.drawString(WiFi.localIP().toString(),64,64);
  epaper.update();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";
    int contentLength = 0;
       


    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();

      if (client.available()) {             // if there's bytes to read from the client,
        currentLine = client.readStringUntil('\n');   // readLine
        currentLine.trim();                           // get rid of \r 
        Serial.println(currentLine);                  // DEBUG: printout of received line
        header += currentLine;
        
        // if the currentLine contains the Content-Length, extract that int
        if (currentLine.startsWith("Content-Length:")){
          currentLine = currentLine.substring(15);
          currentLine.trim();
          contentLength = currentLine.toInt();
        }

        // if the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
        if (currentLine.length() == 0) {
          if (contentLength != 0)  {
            // falls der Request einen body hat reserviere genug speicher für einen String in ContentLength 
            body = "";
            body.reserve(contentLength);
            
            // lies den body ein bis die contentlength erreicht ist
            while (body.length() < contentLength){
              if (client.available()){
                body += (char)client.read();
              }
            }
          }
          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
          // and a content-type so the client knows what's coming, then a blank line:
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/plain");
          client.println("Content-Length: 16");            
          client.println("Connection: close");
          client.println();
          client.println("Request received");
          // Break out of the while loop
          break;
        }
        else {currentLine = "";}
      } 
    }
    if(header.indexOf("Content-Type: text/shrimp") > -1){
      //CMD1: xxx yyy String; (Draw String at (int x, int y))
      int index = body.indexOf("CMD1: ");
      if( index > -1){
        int x = body.substring(index+6, index+9).toInt();
        int y = body.substring(index+10, index+13).toInt();
        int separator = body.indexOf(";", index);
        String text = body.substring(index+14, separator);
        epaper.drawString(text, x, y);
      }
      //CMD2: i; (setFont(int i corresponds to font))
      int index = body.indexOf("CMD2: ");
      if( index > -1){
        switch (unsigned int i = (unsigned int)(body[index+6])){
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
      }
      //CMD3: hh:mm (setTime(char[3]hh, char[3]mm)
      if(int index = body.indexOf("CMD3: "); index > -1){
        char new_hour[3] = body[index+6];index+7};
        char new_min[3] = body{index+9,index+10};
        setTime(new_hour, new_min);
      
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
