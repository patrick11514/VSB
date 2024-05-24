#pragma once

#include <optional>
#include <sys/socket.h>

#include <string>
#include <unordered_map>

#include "../utils/utils.hpp"

/**
 * @brief HTTP Response from server
 */
class HTTPResponse
{
    std::string separator = "\r\n";
    std::string version; ///< http version
    std::optional<std::string> codeText = std::nullopt;

    std::string codeToText() const;                                   ///< convert HTTP code to status text
    std::string formatDefaultPage(const std::string &codeText) const; ///< format default error page from htmls/default_code.html

public:
    unsigned short code; ///< response code
    //
    std::unordered_map<Header, std::string> headers; ///< headers
    //
    std::string_view content = ""; ///< content of response
    //
    bool isValid = true;
    //
    bool addCustomContent = true; ///< if we want to add custom content like error page on send

    HTTPResponse(std::string version, unsigned short code); ///< constructor
    HTTPResponse(const std::string &string);                ///< Parse response to structure

    bool send(int fd) const; ///< send response to client
};
