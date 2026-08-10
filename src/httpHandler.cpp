#include <HTTPClient.h>
#include "Secrets.h"
#include "Certificate.h"
#include "httpHandler.h"

HTTPClient httpclient;
WiFiClientSecure client;
int statuscode;
const char* headers[] = {"Content-Type", "Content-Size", "X-Error-Message:"};

void sendRequest(uint8_t* payload , uint8_t payloadSize){
    client.setCACert(ROOT_CA);
    httpclient.setAuthorization(HTTP_AUTH_NAME, HTTP_AUTH_PWD);
    httpclient.begin(client, MY_SERVER_API);
    httpclient.addHeader("Content-Type", "application/x-shrimp");
    int statuscode = httpclient.POST(payload,payloadSize);
}



