#include "devMode.hpp"

#include "../../utils/decode.hpp"

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
