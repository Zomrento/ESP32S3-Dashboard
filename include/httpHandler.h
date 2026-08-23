#pragma once
#include <stdio.h>

/// @brief sends a secure HTTPS request to the server
/// @param payload HTTP body
/// @param payloadSize HTTP body size
void sendHTTPSRequest(uint8_t* payload , uint8_t payloadSize);

/// @brief a struct to aggregate the results of a HTTP request
struct HTTPSResult{
    HTTPSResult();
    int statuscode;
    String content;
    String contentType;
    uint contentLength;
    String x_error;
};

String sendHTTPRequest(String _URL);

/// @brief getter for the HTTPResult-struct
/// @return the in this project initialized HTTPResult-object
HTTPSResult getHTTPSResult();