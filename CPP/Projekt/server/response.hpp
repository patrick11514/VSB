#pragma once

#include <sys/socket.h>

#include <string>
#include <unordered_map>

/**
 * @brief HTTP Response from server
 */
class HTTPResponse
{
    std::string separator = "\r\n";
    std::string version; ///< http version

    std::string codeToText() const;                                   ///< convert HTTP code to status text
    std::string formatDefaultPage(const std::string &codeText) const; ///< format default error page from htmls/default_code.html

public:
    unsigned short code; ///< response code
    //
    std::unordered_map<std::string, std::string> headers; ///< headers
    //
    std::string_view content = ""; ///< content of response

    HTTPResponse(std::string version, unsigned short code); ///< constructor

    bool send(int fd) const; ///< send response to client
};
