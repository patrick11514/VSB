#pragma once

#include <string_view>
#include <vector>
#include <unordered_map>

#include "../socket/socket.hpp"

class HTTPPayload
{
    std::string_view payload;

public:
    //
    std::string_view method;
    std::string_view path;
    std::string_view httpVersion;
    //
    std::unordered_map<std::string_view, std::string_view> headers;
    //
    std::string_view content;
    //
    bool isValid = false;

    HTTPPayload(const ReceivedData &data);
};