#include "devMode.hpp"

#include "../../utils/utils.hpp"
#include "../server.hpp"

DevMode::DevMode(const ArgParser &parser, Logger *logger) : MainMode(parser, logger), indexes{"index.html", "index.htm"}
{
    if (parser.includes("path"))
    {
        this->path = fs::canonical(*parser.getByKey("path"));
    }
    else
    {
        this->path = fs::current_path();
    }
}

DevMode::~DevMode() {}

void DevMode::handleRequest(const ReceivedData &client, const HTTPPayload &data)
{
    std::string_view ipaddress = client.address;
    auto header = data.headers.find("X-Forwarded-For");

    if (header != data.headers.end())
    {
        ipaddress = header->second;
    }

    fs::path filePath = this->path / decode(data.path);

    FileRead file(filePath);

    if (!file.exists())
    {
        HTTPResponse response(std::string(data.httpVersion), 404);
        response.send(client.fd);
        return;
    }

    HTTPResponse response(std::string(data.httpVersion), 200);
    response.headers.emplace("Server", "Tondik/" + Server::version);

    if (!file.isFolder())
    {
        this->logger->info(std::format("{} to {} from {}", data.method, data.path, ipaddress));
        this->doFile(filePath, data, response, file, client.fd);
    }
    else
    {
        bool found = false;
        for (const auto &index : this->indexes)
        {
            fs::path newPath = filePath / index;

            if (fs::exists(newPath))
            {
                std::string newPathToConsole = std::string(data.path);
                newPathToConsole.append(index);
                this->logger->info(std::format("{} to {} from {}", data.method, newPathToConsole, ipaddress));
                this->doFile(newPath, data, response, FileRead(newPath), client.fd);
                found = true;
                break;
            }
        }

        if (!found)
        {
            this->logger->info(std::format("{} to {} from {}", data.method, data.path, ipaddress));
            response.code = 404;
            response.send(client.fd);
        }
    }

    // close connection with client
    close(client.fd);
}
