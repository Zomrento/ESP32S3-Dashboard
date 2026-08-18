#include <HTTPClient.h>
#include "Secrets.h"
#include "Certificate.h"
#include "httpHandler.h"

HTTPClient httpclient;
WiFiClientSecure client;
HTTPResult httpresult;

// Array of the headers I want to collect later. Apparently they have to be specified beforehand
const char* headers[] = {"Content-Type", "Content-Length", "X-Error-Message"};


// A small struct to aggregate HTTPResponses
HTTPResult::HTTPResult()
{
    statuscode = 0;
    content = "NO DATA";
    contentType = "text/plain";
    contentLength = 7;
    x_error = "";
}


String sendHTTPRequest(String _URL){
    httpclient.begin(_URL);
    return(httpclient.getString());
}

void sendHTTPSRequest(uint8_t* payload , uint8_t payloadSize){
    
    client.setCACert(ROOT_CA);
    // AUTH for the Server to connect to set in Secret.h
    httpclient.setAuthorization(HTTP_AUTH_NAME, HTTP_AUTH_PWD);
    // SERVER_API set in Secret.h
    httpclient.begin(client, MY_SERVER_API);
    // Set the Content_Type to my own protocol
    httpclient.addHeader("Content-Type", "application/x-shrimp");
    // Announce the headers I want to collect using the Array "headers"
    httpclient.collectHeaders(headers, 3);
    // The POST function returns on its own just the Statuscode (in example 200 OK 404 Not found etc.)
    httpresult.statuscode = httpclient.POST(payload,payloadSize);
    // Set full 
    httpresult.contentType = httpclient.header("Content-Type");
    httpresult.contentLength = httpclient.header("Content-Length").toInt();
    httpresult.x_error = httpclient.header("X-Error-Message");
    httpresult.content = httpclient.getString();
    httpclient.end();
}

HTTPResult getHTTPResult(){
    return httpresult;
}
