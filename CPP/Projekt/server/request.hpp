#pragma once

#include "payload.hpp"
#include "../utils/utils.hpp"
#include <unordered_map>

class HTTPRequest {
    std::string separator = "\r\n";

    std::string method;
    std::string path;
    std::string httpVersion;
    std::unordered_map<Header, std::string> headers;
    std::string content;

    public:
    //Create request and send it to specific ip+port + add forwarded headers
    HTTPRequest(const HTTPPayload& payload);

    void addHeader(std::string key, std::string value);
    bool send(int fd) const;
};
