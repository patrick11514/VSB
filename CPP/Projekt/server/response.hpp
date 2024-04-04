#pragma once

#include <sys/socket.h>

#include <string>
#include <unordered_map>

class HTTPResponse
{
    std::string separator = "\r\n";
    std::string version;

    std::string codeToText() const;
    std::string formatDefaultPage(const std::string &codeText) const;

public:
    unsigned short code;
    //
    std::unordered_map<std::string, std::string> headers;
    //
    std::string_view content = "";

    HTTPResponse(std::string version, unsigned short code);

    bool send(int fd) const;
};
