#include "devMode.hpp"

#include <fstream>

void DevMode::doFile(const fs::path &filePath, const HTTPPayload &payload, HTTPResponse &response, const FileRead &file, int fd) const
{
    uintmax_t fileSize = fs::file_size(filePath);

    std::ifstream iFile(filePath);
    // 1024 * 1024
    char data[1048576];

    bool startSet = false;
    size_t startAt = 0;
    size_t end = fileSize;

    // range works only in VLC :(
    /*if (payload.headers.find("Range") != payload.headers.end())
    {
        std::string_view range = payload.headers.at("Range");

        std::cout << range << std::endl;

        if (!range.starts_with("bytes="))
        {
            // bad request
            response.code = 400;
            return;
        }

        size_t eq = range.find("=");
        std::string_view data(range.begin() + eq + 1, range.end());

        size_t dash = data.find("-");
        if (dash == std::string_view::npos)
        {
            // bad request
            response.code = 400;
            return;
        }

        std::string_view from(data.begin(), data.begin() + dash);
        std::string_view to(data.begin() + dash + 1, data.end());

        if (from.size() > 0)
        {
            startSet = true;
            startAt = static_cast<size_t>(std::stoi(std::string(from)));
        }

        if (to.size() > 0)
        {
            std::string_view to(data.begin() + dash + 1, data.end());
            end = static_cast<size_t>(std::stoi(std::string(to)));
        }

        // partial content
        response.code = 206;
    }
    else
    {
        response.headers.emplace("Accept-Ranges", "bytes");
    }*/

    // send empty response with correct length
    if (startSet)
    {
        response.headers.emplace("Content-Range",
                                 std::format("bytes {}-{}/{}", std::to_string(startAt), std::to_string(end), std::to_string(fileSize)));
    }

    response.headers.emplace("Content-Length", std::to_string(end - startAt));
    response.headers.emplace("Content-Type", file.getMimeType());
    // response.headers.emplace("Connection", "close");
    response.send(fd);

    iFile.seekg(startAt);
    size_t read = 0;

    while (!(iFile.eof() || iFile.fail()))
    {
        size_t toRead = sizeof(data);

        size_t diff = end - read;
        if (diff < toRead)
        {
            toRead = static_cast<size_t>(diff);
        }

        iFile.read(data, toRead);

        size_t readed = iFile.gcount();

        if ((int)::send(fd, data, readed, 0) == -1)
        {
            // error or user just canceled request
            return;
        }

        read += readed;

        if (read > static_cast<size_t>(end))
        {
            return;
        }

        // Get new test
        // std::cout << (int)recv(fd, &buffer, sizeof(buffer), MSG_DONTWAIT) << std::endl;
    }
}

DevMode::DevMode(const ArgParser &parser, Logger &logger) : MainMode(parser, logger), indexes{"index.html", "index.htm"}
{
    if (parser.includes("path"))
    {
        this->path = fs::absolute(*parser.getByKey("path"));
    }
    else
    {
        this->path = fs::current_path();
    }
}

void DevMode::handleRequest(const ReceivedData &client, const HTTPPayload &data)
{
    fs::path filePath = this->path;
    filePath += decode(data.path);

    FileRead file(filePath);

    if (!file.exists())
    {
        HTTPResponse response(std::string(data.httpVersion), 404);
        response.send(client.fd);
        return;
    }

    HTTPResponse response(std::string(data.httpVersion), 200);
    response.headers.emplace("Server", "Tondik/1.0");

    if (!file.isFolder())
    {
        this->logger.info(std::format("{} to {} from {}", data.method, data.path, client.address));
        this->doFile(filePath, data, response, file, client.fd);
    }
    else
    {
        bool found = false;
        for (const auto &index : this->indexes)
        {
            fs::path newPath = filePath;
            newPath.concat(index);

            if (fs::exists(newPath))
            {
                std::string newPathToConsole = std::string(data.path);
                newPathToConsole.append(index);
                this->logger.info(std::format("{} to {} from {}", data.method, newPathToConsole, client.address));
                this->doFile(newPath, data, response, FileRead(newPath), client.fd);
                found = true;
                break;
            }
        }

        if (!found)
        {
            this->logger.info(std::format("{} to {} from {}", data.method, data.path, client.address));
            response.code = 404;
            response.send(client.fd);
        }
    }

    // close connection with client
    close(client.fd);
}
