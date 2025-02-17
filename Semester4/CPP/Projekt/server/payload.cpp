#include "payload.hpp"
#include "server.hpp"

HTTPPayload::HTTPPayload(const ReceivedData &data)
{
    this->payload = data.data;

    std::string_view rest{this->payload.begin(), this->payload.end()};

    /// PARSE FIRST LINE HTTP/VER CODE TEXT

    auto endOfLine = rest.find(this->separator);
    if (endOfLine == rest.npos)
    {
        this->isValid = false;
        return;
    }

    std::string_view versionCode{rest.begin(), rest.begin() + endOfLine};

    std::vector<std::string_view> parts;
    parts.reserve(3);

    while (true)
    {
        auto split = versionCode.find(' ');
        if (split == versionCode.npos || parts.size() > 1)
        {
            if (parts.size() < 1)
            {
                this->isValid = false;
                return;
            }

            parts.push_back({versionCode.begin(), versionCode.end()});
            break;
        }

        parts.push_back({versionCode.begin(), versionCode.begin() + split});
        versionCode = {versionCode.begin() + split + 1, versionCode.end()};
    }

    this->method = parts[0];
    std::string_view tempPath = parts[1];

    if (tempPath.find("..") != tempPath.npos)
    {
        isValid = false;
        return;
    }

    if (tempPath[0] == '/')
    {
        this->path = std::string_view(tempPath.begin() + 1, tempPath.end());
    }
    else
    {
        this->path = tempPath;
    }
    this->httpVersion = parts[2];

    /// PARSE HEADERS KEY:VALUE\r\nKEY:VALUE\r\n...

    rest = std::string_view{rest.begin() + endOfLine + this->separator.size(), rest.end()};

    auto endOfHeaders = rest.find(std::format("{}{}", this->separator, this->separator));
    if (endOfHeaders == rest.npos)
    {
        this->isValid = false;
        return;
    }

    std::string_view headers{rest.begin(), rest.begin() + endOfHeaders};

    /// Put headers to map

    while (true)
    {
        auto sep = headers.find(this->separator);
        bool end = false;

        std::string_view keyValue;

        if (sep == headers.npos)
        {
            end = true;

            keyValue = headers;
        }
        else
        {
            keyValue = {headers.begin(), headers.begin() + sep};
        }

        auto colon = keyValue.find(": ");

        if (colon == keyValue.npos)
        {
            this->isValid = false;
            return;
        }

        this->headers.emplace(
            Header{std::string_view{keyValue.begin(), keyValue.begin() + colon}},
            std::string{
                keyValue.begin() + colon + 2,
                keyValue.end()});

        if (end)
        {
            break;
        }

        headers = {headers.begin() + sep + this->separator.size(), headers.end()};
    }

    this->content = {rest.begin() + endOfHeaders + this->separator.size() * 2, rest.end()};

    this->isValid = true;
}
