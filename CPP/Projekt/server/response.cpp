#include <format>
#include <string>

#include "response.hpp"
#include "server.hpp"

std::string HTTPResponse::codeToText() const
{
    // https://developer.mozilla.org/en-US/docs/Web/HTTP/Status
    std::unordered_map<unsigned int, std::string> data{
        {200, "OK"},

        {201, "Created"},

        {202, "Accepted"},
        {203, "Non-Authoritative Information"},
        {200, "OK"},
        {204, "No Content"},
        {205, "Reset Content"},
        {206, "Partial Content"},

        {207, "Multi-Status"},
        {208, "Already Reported"},

        {226, "IM Used"},

        {300, "Multiple Choices"},
        {301, "Moved Permanently"},
        {302, "Found"},
        {303, "See Other"},
        {304, "Not Modified"},
        {305, "Use Proxy"},
        {306, "unused"},
        {307, "Temporary Redirect"},
        {302, "Found"},

        {308, "Permanent Redirect"},

        {301, "Moved Permanently"},

        {400, "Bad Request"},
        {401, "Unauthorized"},
        {402, "Payment Required"},
        {403, "Forbidden"},
        {401, "Unauthorized"},
        {404, "Not Found"},
        {403, "Forbidden"},
        {405, "Method Not Allowed"},

        {406, "Not Acceptable"},
        {407, "Proxy Authentication Required"},
        {401, "Unauthorized"},
        {408, "Request Timeout"},
        {409, "Conflict"},
        {410, "Gone"},
        {411, "Length Required"},

        {412, "Precondition Failed"},
        {413, "Payload Too Large"},

        {414, "URI Too Long"},
        {415, "Unsupported Media Type"},
        {416, "Range Not Satisfiable"},

        {417, "Expectation Failed"},

        {418, "I'm a teapot"},
        {421, "Misdirected Request"},
        {422, "Unprocessable Content"},
        {423, "Locked"},
        {424, "Failed Dependency"},
        {425, "Too Early"},
        {426, "Upgrade Required"},

        {428, "Precondition Required"},

        {429, "Too Many Requests"},
        {431, "Request Header Fields Too Large"},
        {451, "Unavailable For Legal Reasons"},
        {500, "Internal Server Error"},
        {501, "Not Implemented"},

        {502, "Bad Gateway"},
        {503, "Service Unavailable"},

        {504, "Gateway Timeout"},
        {505, "HTTP Version Not Supported"},
        {506, "Variant Also Negotiates"},
        {507, "Insufficient Storage"},
        {508, "Loop Detected"},
        {510, "Not Extended"},
        {511, "Network Authentication Required"}};

    try
    {
        return data.at(this->code);
    }
    catch (const std::out_of_range &)
    {
        return "";
    }
}

std::string HTTPResponse::formatDefaultPage(const std::string &codeText) const
{
    std::string style("html, body {margin:0;padding: 0;width: 100%;height:100%;}body {display:flex;align-items: center;flex-direction: column;}");

    return std::format("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>{} {}</title><style>{}</style></head><body><h1>{} {}</h1><hr style=\"width: 100%;\"><p>Tondík v{}</p></body></html>",
                       this->code, codeText, style, this->code, codeText, Server::version);
}

HTTPResponse::HTTPResponse(std::string version, unsigned short code) : version(version), code(code) {}

HTTPResponse::HTTPResponse(const std::string &data)
{
    std::string_view rest{data.begin(), data.end()};

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

    this->version = parts[0];
    this->code = static_cast<unsigned short>(std::stoul(std::string{parts[1]}));

    if (parts.size() > 2)
    {
        this->codeText = parts[2];
    }

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
}

bool HTTPResponse::send(int fd) const
{
    std::string codeText;
    if (this->codeText == std::nullopt)
    {
        codeText = this->codeToText();
    }
    else
    {
        codeText = this->codeText.value();
    }
    std::string firstLine = std::format("{} {} {}", this->version, this->code, codeText);
    std::string headers;

    for (auto header : this->headers)
    {
        headers.append(std::format("{}: {}{}", header.first.getOriginal(), header.second, this->separator));
    }

    std::string generated;
    if (this->content.length() == 0 && this->code >= 300 && this->addCustomContent == true)
    {
        std::string content = this->formatDefaultPage(codeText);
        if (this->headers.find(Header{"Content-Length"}) == this->headers.end())
        {
            headers.append(std::format("Content-Length: {}{}", content.length(), this->separator));
        }
        generated = std::format("{}{}{}{}{}",
                                firstLine, this->separator, headers, this->separator,
                                content);
    }
    else
    {
        if (this->headers.find(Header{"Content-Length"}) == this->headers.end())
        {
            headers.append(std::format("Content-Length: {}{}", this->content.length(), this->separator));
        }
        generated = std::format("{}{}{}{}{}", firstLine, this->separator, headers, this->separator, this->content);
    }

    return static_cast<int>(::send(fd, generated.data(), generated.size(), 0)) != -1;
}
