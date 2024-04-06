#include "payload.hpp"
#include "server.hpp"

HTTPPayload::HTTPPayload(const ReceivedData &data)
{
    this->payload = data.data;
    std::string_view restOfData = this->payload;
    std::vector<std::string_view> sws;

    while (true)
    {
        size_t occurence = restOfData.find("\r\n");

        if (occurence == std::string_view::npos)
        {
            sws.push_back(std::string_view(restOfData.begin(), restOfData.end()));
            break;
        }

        sws.push_back(std::string_view(restOfData.begin(), restOfData.begin() + occurence));
        restOfData = std::string_view(restOfData.begin() + occurence + 2, restOfData.end());
    }

    // parse first line
    std::vector<std::string_view> httpParts;
    std::string_view part = sws[0];
    httpParts.reserve(3);

    while (true)
    {
        size_t space = part.find(' ');
        if (space == std::string_view::npos)
        {
            httpParts.push_back(std::string_view(part.begin(), part.end()));
            break;
        }

        httpParts.push_back(std::string_view(part.begin(), part.begin() + space));
        part = std::string_view(part.begin() + space + 1, part.end());
    }

    if (httpParts.size() < 3)
    {
        return;
    }

    this->method = httpParts[0];
    std::string_view tempPath = httpParts[1];

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
    this->httpVersion = httpParts[2];

    // headers
    size_t i = 1;
    for (; i < sws.size(); ++i)
    {
        if (sws[i].size() == 0)
        {
            break;
        }
        std::string_view &row = sws[i];
        size_t separator = row.find(": ");
        if (separator == std::string_view::npos)
        {
            if (Server::instance != nullptr)
            {
                Server::instance->l->warn(std::format("Cannot parse header: {}", row));
            }
            continue;
        }

        this->headers.emplace(
            std::string_view(row.begin(), row.begin() + separator),
            std::string_view(row.begin() + separator + 2, row.end()));
    }

    this->content = sws[i + 1];

    this->isValid = true;
}
