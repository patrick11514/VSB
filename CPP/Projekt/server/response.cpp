#include "response.hpp"

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

    return std::format("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>{} {}</title><style>{}</style></head><body><h1>{} {}</h1><hr style=\"width: 100%;\"><p>Tondík v1.0</p></body></html>",
                       this->code, codeText, style, this->code, codeText);
}

HTTPResponse::HTTPResponse(std::string version, unsigned short code) : version(version), code(code) {}

bool HTTPResponse::send(int fd) const
{
    std::string codeText = this->codeToText();
    std::string firstLine = std::format("{} {} {}", this->version, this->code, codeText);
    std::string headers;

    std::string_view a("ahoj");
    std::string_view b("ahoj");

    for (auto header : this->headers)
    {
        headers.append(std::format("{}: {}{}", header.first, header.second, this->separator));
    }

    std::string generated;
    if (this->content.length() == 0 && this->code >= 300)
    {
        std::string content = this->formatDefaultPage(codeText);
        headers.append(std::format("Content-Length: {}{}", content.length(), this->separator));
        generated = std::format("{}{}{}{}{}",
                                firstLine, this->separator, headers, this->separator,
                                content);
    }
    else
    {
        headers.append(std::format("Content-Length: {}{}", this->content.length(), this->separator));
        generated = std::format("{}{}{}{}{}", firstLine, this->separator, headers, this->separator, this->content);
    }

    return (int)::send(fd, generated.data(), generated.size(), 0) != -1;
}
