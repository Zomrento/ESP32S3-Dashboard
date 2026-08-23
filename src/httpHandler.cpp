#include <HTTPClient.h>
#include "Secrets.h"
#include "Certificate.h"
#include "httpHandler.h"

HTTPClient httpclient;
HTTPClient httpsclient;
WiFiClientSecure client;
HTTPSResult httpsresult;

// Array of the headers I want to collect later. Apparently they have to be specified beforehand
const char* headers[] = {"Content-Type", "Content-Length", "X-Error-Message"};


// A small struct to aggregate HTTPResponses
HTTPSResult::HTTPSResult()
{
    statuscode = 0;
    content = "NO DATA";
    contentType = "text/plain";
    contentLength = 7;
    x_error = "";
}

// Func to reach simple HTTP APIs
String sendHTTPRequest(String _URL){
    httpclient.begin(_URL);
    Serial.println(httpclient.GET());
    String text = httpclient.getString();
    Serial.println("String is:" + text);
    httpclient.end();
    return(text);
}

void sendHTTPSRequest(uint8_t* payload , uint8_t payloadSize){
    // ROOT_CertAuth defined in Secret.h
    client.setCACert(ROOT_CA);
    // AUTH for the Server to connect to defined in Secret.h
    httpsclient.setAuthorization(HTTP_AUTH_NAME, HTTP_AUTH_PWD);
    // SERVER_API defined in Secret.h
    httpsclient.begin(client, MY_SERVER_API);
    // Set the Content_Type to my own protocol
    httpsclient.addHeader("Content-Type", "application/x-shrimp");
    // Announce the headers I want to collect using the Array "headers"
    httpsclient.collectHeaders(headers, 3);
    // The POST function returns on its own just the Statuscode (in example 200 OK 404 Not found etc.)
    httpsresult.statuscode = httpsclient.POST(payload,payloadSize);
    // Set full 
    httpsresult.contentType = httpsclient.header("Content-Type");
    httpsresult.contentLength = httpsclient.header("Content-Length").toInt();
    httpsresult.x_error = httpsclient.header("X-Error-Message");
    httpsresult.content = httpsclient.getString();
    Serial.println(httpsresult.statuscode);
    Serial.println(httpsresult.contentType);
    Serial.println(httpsresult.x_error);
    Serial.println(httpsresult.content);
    httpsclient.end();
}
/// @brief Getter for the httpsresult struct
/// @return the aggregated requestresponse
HTTPSResult getHTTPSResult(){
    return httpsresult;
}
