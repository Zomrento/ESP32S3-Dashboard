#include <HTTPClient.h>
#include "Secrets.h"
#include "Certificate.h"
#include "httpHandler.h"

HTTPClient httpclient;
WiFiClientSecure client;
HTTPResult httpresult;
const char* headers[] = {"Content-Type", "Content-Length", "X-Error-Message"};

HTTPResult::HTTPResult()
{
    statuscode = 0;
    content = "NO DATA";
    contentType = "text/plain";
    contentLength = 7;
    x_error = "";
}

void sendRequest(uint8_t* payload , uint8_t payloadSize){
    client.setCACert(ROOT_CA);
    httpclient.setAuthorization(HTTP_AUTH_NAME, HTTP_AUTH_PWD);
    httpclient.begin(client, MY_SERVER_API);
    httpclient.addHeader("Content-Type", "application/x-shrimp");
    httpclient.collectHeaders(headers, 3);
    httpresult.statuscode = httpclient.POST(payload,payloadSize);
    httpresult.contentType = httpclient.header("Content-Type");
    httpresult.contentLength = httpclient.header("Content-Length").toInt();
    httpresult.x_error = httpclient.header("X-Error-Message");
    httpresult.content = httpclient.getString();
}

HTTPResult getHTTPResult(){
    return httpresult;
}
