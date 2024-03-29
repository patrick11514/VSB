#include "devMode.hpp"

#include <fstream>

void DevMode::doFile(const fs::path &filePath, HTTPResponse &response, const FileRead &file, int fd) const
{
    std::ifstream iFile(filePath);
    std::stringstream ss;

    ss << iFile.rdbuf();

    std::string data = ss.str();
    response.content = data;

    response.headers.emplace("Content-Type", file.getMimeType());

    response.send(fd);
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
    filePath.concat(data.path);

    FileRead file(filePath);

    if (!file.exists())
    {
        HTTPResponse response(std::string(data.httpVersion), 404);
        response.send(client.fd);
        return;
    }

    HTTPResponse response(std::string(data.httpVersion), 200);

    if (!file.isFolder())
    {
        this->logger.info(std::format("{} to {} from {}", data.method, data.path, client.address));
        this->doFile(filePath, response, file, client.fd);
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
                this->doFile(newPath, response, FileRead(newPath), client.fd);
                found = true;
                break;
            }
        }

        if (!found)
        {
            response.code = 404;
            response.send(client.fd);
        }
    }

    // send(clientData.fd, payload.content.data(), payload.content.size(), 0);
    // send(clientData.fd, str.data(), str.size(), 0);
    // close connection with client
    close(client.fd);
}
