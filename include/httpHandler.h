#pragma once
#include <stdio.h>

/// @brief sends a secure HTTPS request to the server
/// @param payload HTTP body
/// @param payloadSize HTTP body size
void sendRequest(uint8_t* payload , uint8_t payloadSize);

/// @brief a struct to aggregate the results of a HTTP request
struct HTTPResult{
    HTTPResult();
    int statuscode;
    String content;
    String contentType;
    uint contentLength;
    String x_error;
};

/// @brief getter for the HTTPResult-struct
/// @return the in this project initialized HTTPResult-object
HTTPResult getHTTPResult();