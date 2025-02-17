#include "request.hpp"
#include <format>

HTTPRequest::HTTPRequest(const HTTPPayload &payload) : method(payload.method), path(payload.path), httpVersion(payload.httpVersion), content(payload.content)
{
    for (auto header : payload.headers)
    {
        this->headers.emplace(header.first, header.second);
    }
}

void HTTPRequest::addHeader(std::string key, std::string value)
{
    this->headers.emplace(key, value);
}

bool HTTPRequest::send(int fd) const
{
    std::string mainPart = std::format("{} /{} {}", this->method, this->path, this->httpVersion);
    std::string headers;
    for (auto &header : this->headers)
    {
        headers += std::format("{}: {}{}", header.first.getOriginal(), header.second, this->separator);
    }

    std::string final = std::format("{}{}{}{}{}", mainPart, this->separator, headers, this->separator, this->content);

    return ::send(fd, final.data(), final.size(), 0) == -1;
}
