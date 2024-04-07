#pragma once

#include <string_view>
#include <vector>
#include <unordered_map>

#include "../socket/socket.hpp"

/**
 * @brief HTTP Payload from client
 */
class HTTPPayload
{
    std::string_view payload; ///< full payload

public:
    //
    std::string_view method;      ///< HTTP Method (GET, POST...)
    std::string_view path;        ///< path from request
    std::string_view httpVersion; ///< http version
    //
    std::unordered_map<std::string_view, std::string_view> headers; ///< headers
    //
    std::string_view content; ///< content of request
    //
    bool isValid = false; ///< if payload is valid

    HTTPPayload(const ReceivedData &data); ///< constructor
};