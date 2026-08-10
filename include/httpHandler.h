#pragma once
#include <stdio.h>
void sendRequest(uint8_t* payload , uint8_t payloadSize);

struct HTTPResult{
    HTTPResult();
    int statuscode;
    String content;
    String contentType;
    uint contentLength;
    String x_error;
};

HTTPResult getHTTPResult();