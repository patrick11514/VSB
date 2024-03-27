#pragma once

#include <sys/socket.h>

#include <string>
#include <unordered_map>
#include <format>

class HTTPResponse
{
    std::string separator = "\r\n";
    std::string version;
    unsigned short code;

    std::string codeToText() const;

public:
    //
    std::unordered_map<std::string, std::string> headers;
    //
    std::string_view content;

    HTTPResponse(std::string version, unsigned short code);

    bool send(int fd) const;
};
