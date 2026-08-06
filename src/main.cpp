#include "driver.h"
#include "WiFi.h"
#include "TFT_eSPI.h"
#include "Secrets.h"
#include "fonts.h"
#include "dashboard.h"
#include "shrimp.h"

// Set up EPaper object
EPaper epaper;
// Set web server port number to 80
WiFiServer server(80);
// Variable to store the HTTP request
String header;
String body;
boolean header_finished;
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
    header_finished = false;
    String currentLine = "";
    int contentLength = 0;
       


    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();

      if (client.available()) {             // if there's bytes to read from the client,
        if(!header_finished){
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
          // that's the end of the client HTTP header
          if (currentLine.length() == 0) {
            header_finished = true;
          }
          else {
            currentLine = "";
          }
        }
        else{
          if (contentLength != 0)  {
            /* To spontainiously display small changes on the dashboard I make some commands
               callable over http. (Also as an excercise, because I never did this before.)
               NOTE: For selfdefined protocols the prefixes:
                    application/.vnd- (vendor)
                    application/x-    (not-standarized)
                    seem to be common. Also for pure bytestreams
                    application/octet-stream seems to be an option
                    I choose for my own small protocol-exercise the name application/x-shrimp
                    Commands will arrive in the format CMDLENGTH CMDID DATA*
                    CMDLENGTH tells the receiver how many bytes AFTER CMDLENGTH still belong to that command. (So DATA.length() + 1 to account for the CMD-Byte)
                    CMDID identifies the command (Example: 0x01 means drawString, 0x02 means setFont)
                    DATA contain for the command neccesary additionaly information, (Example: Text, (x,y)-coordinates)
            */
            if(header.indexOf("Content-Type: application/x-shrimp")){
              uint16_t bodyLengthCounter = 0;
              uint8_t command[255];
              command[0] += client.read();
              while(bodyLengthCounter < contentLength){
                // command + 1 should be the address of command[1] so the CMDLength in [0] is kept untouched 
                client.readBytes(command+1, command[0]);
                // command[0]+1 = (DATALENGTH + CMDBYTE) + CMDLENGTHBYTE
                bodyLengthCounter += command[0]+1;
                shrimpCMD(command, epaper);
              }
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/plain");
              client.println("Content-Length: 16");            
              client.println("Connection: close");
              client.println();
              client.println("Request received");
              break;
            }
            else{
              client.println("HTTP/1.1 400 Bad Request");
              client.println("Content-Type: text/plain");
              client.println("Content-Length: 18");            
              client.println("Connection: close");
              client.println();
              client.println("Invalid Operation");
              break;
            }
          }
        }
      }
    } 
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
