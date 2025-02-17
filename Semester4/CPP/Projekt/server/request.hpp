#pragma once

#include "payload.hpp"
#include "../utils/utils.hpp"
#include <unordered_map>

class HTTPRequest
{
    std::string separator = "\r\n"; ///< HTTP head separator

    std::string method;                              ///< HTTP Method name
    std::string path;                                ///< Path in browser
    std::string httpVersion;                         ///< HTTP version
    std::unordered_map<Header, std::string> headers; ///< HTTP Headers
    std::string content;                             ///< Content

public:
    // Create request and send it to specific ip+port + add forwarded headers
    HTTPRequest(const HTTPPayload &payload);

    /**
     * @brief Add header to map of headers
     * @param key Name of header
     * @param value Value of header
     */
    void addHeader(std::string key, std::string value);
    /**
     * @brief Send request to socket's fd
     * @param fd File descriptor for socket
     * @return True if request was sucessfully sent
     */
    bool send(int fd) const;
};
