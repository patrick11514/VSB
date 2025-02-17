#pragma once

#include <string_view>
#include <unordered_map>

#include "../socket/socket.hpp"
#include "../utils/utils.hpp"

/**
 * @brief HTTP Payload from client
 */
class HTTPPayload
{
    std::string separator = "\r\n";
    std::string_view payload; ///< full payload

public:
    //
    std::string_view method;      ///< HTTP Method (GET, POST...)
    std::string_view path;        ///< path from request
    std::string_view httpVersion; ///< http version
    //
    std::unordered_map<Header, std::string> headers; ///< headers
    //
    std::string_view content; ///< content of request
    //
    bool isValid = false; ///< if payload is valid

    HTTPPayload(const ReceivedData &data); ///< constructor
};
