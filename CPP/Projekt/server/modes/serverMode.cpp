#include "serverMode.hpp"
#include "../../utils/utils.hpp"
#include "../server.hpp"
#include "../request.hpp"

#include <filesystem>
#include <fstream>
#include <format>
#include <chrono>

void ServerMode::createDefaultHTMLFile(const fs::path &folderPath, const fs::path &mainConfig, const fs::path &domainConfigs) const
{
    fs::create_directory(folderPath);

    // create default HTML file

    std::string fileContent = std::format("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Default Page | Tondik Web Server</title><style>{}</style></head><body><div> <h1>Welcome on Tondík!</h1> <p>If you see this page, your server is working. <br />You need to change domain configuration, or default file directory.<br /><br />Main config path: {}<br />Domains config directory: {} </p> <i>Tondik v{}</i></div></body></html>",
                                          /* CSS, because it contains {...} it need to be separated, to not be treated as format argument -> */ "html,body {margin: 0;padding: 0;width: 100%;height: 100%;}body {display: flex;align-items: start;flex-direction: column;}div {margin: 0 auto;width: min(50vw, 512px);}",
                                          mainConfig.string(), domainConfigs.string(), Server::version);

    std::ofstream index(folderPath / "index.html");
    index << fileContent;
    index.close();
}

ServerMode::ServerMode(const ArgParser &parser, Logger *logger, const std::string &configPath) : MainMode(parser, logger)
{
    fs::path path(fs::canonical(configPath));

    if (!fs::exists(path))
    {
        fs::create_directory(path);
    }

    fs::path mainPath(path / "main.ini");

    if (!fs::exists(mainPath))
    {
        std::ofstream configFile(mainPath);
        fs::path accessLog(path / "access.log");
        fs::path errorLog(path / "error.log");
        fs::path defaultFolder(path / "html");
        configFile << std::format("port=80\nconfig_directory=configs\n\naccess_log={}\nerror_log={}\n\ndefault_root={}", accessLog.string(), errorLog.string(), defaultFolder.string());

        configFile.close();
    }

    this->mainConfig = IniParser(mainPath);

    if (!this->mainConfig.isOpened())
    {
        throw std::runtime_error("Config file cannot be loaded.");
    }

    if (this->mainConfig.includes("access_log") != ValueKind::Value)
    {
        throw std::runtime_error("Config doesn't include acces log path");
    }

    if (this->mainConfig.includes("error_log") != ValueKind::Value)
    {

        throw std::runtime_error("Config doesn't include error log path");
    }

    if (this->mainConfig.includes("default_root") != ValueKind::Value)
    {
        throw std::runtime_error("Config doesn't include default root");
    }

    if (this->mainConfig.includes("config_directory") != ValueKind::Value)
    {
        throw std::runtime_error("Config doesn't include configs directory");
    }

    // if doesn't exists, create it
    fs::path defaultRoot(extractRef(this->mainConfig.getValue("default_root")));
    if (!fs::exists(defaultRoot))
    {
        createDefaultHTMLFile(extractRef(this->mainConfig.getValue("default_root")), mainPath, path / extractRef(this->mainConfig.getValue("config_directory")));
    }

    // cannot be nullptr
    Server *server = Server::instance;

    // bind port
    if (this->mainConfig.includes("port") != ValueKind::Value)
    {
        throw std::runtime_error("Config doesn't include port number");
    }

    int port = std::stoi(this->mainConfig.getValue("port").value());
    if (port < 1 || port > 65353)
    {
        throw std::range_error("Invalid port in range 1-65353");
    }

    this->port = port;

    if (this->mainConfig.includes("local") == ValueKind::Value)
    {
        auto value = extractRef(this->mainConfig.getValue("local"));
        if (value != "true" && value != "false")
        {
            throw std::runtime_error("Invalid value for local: " + value);
        }

        if (value == "true")
        {
            this->local = true;
        }
    }

    // load configs
    auto configDirectory = this->mainConfig.getValue("config_directory");
    if (!configDirectory.has_value())
    {
        throw std::runtime_error("Config doesn't include config directory");
    }

    fs::path configsPath(path / extractRef(configDirectory));

    if (!fs::exists(configsPath))
    {
        fs::create_directory(configsPath);
    }

    for (auto configPath : fs::recursive_directory_iterator(configsPath))
    {
        if (configPath.is_directory())
        {
            continue;
        }

        if (configPath.path().extension() != ".ini")
        {
            continue;
        }

        std::string path = configPath.path().string();

        IniParser configParser(configPath.path());

        if (configParser.includes("domain") != ValueKind::Value)
        {
            throw std::runtime_error(std::format("Cannot find domain entry in {}", path));
        }

        auto rootIncludes = configParser.includes("root");
        auto proxyIncludes = configParser.includes("reverse_proxy");

        if (rootIncludes != ValueKind::Value && proxyIncludes != ValueKind::Value)
        {
            throw std::runtime_error(std::format("Config file {} doesn't include root niether reverse_proxy", path));
        }

        if (rootIncludes == ValueKind::Value && proxyIncludes == ValueKind::Value)
        {
            throw std::runtime_error(std::format("Config file {} cannot have root and reverse_proxy at the same time", path));
        }

        std::string key{extractRef(configParser.getValue("domain"))};
        this->domainConfigs.emplace(std::move(key), HostConfig{std::move(configParser)});
    }

    // swap logger at the end
    // because we store original streams in ServerMode and we are passing referenes to Logger
    // if this constructor throws error, references will be invalid, and error cannot be written into
    // file

    this->accessLog = std::ofstream(this->mainConfig.getValue("access_log").value(), std::fstream::app);
    this->errorLog = std::ofstream(this->mainConfig.getValue("error_log").value(), std::fstream::app);

    Logger *newMainLogger = new Logger(this->accessLog, this->errorLog, this->accessLog);
    // swap pointers and delete old logger
    std::swap(server->l, newMainLogger);
    this->logger = server->l;
    delete newMainLogger;

    server->l->info(std::format("Loaded {} domain config files", this->domainConfigs.size()));

    if (this->mainConfig.includes("headers") == ValueKind::Array)
    {
        for (auto &header : extractRef(this->mainConfig.getArray("headers")))
        {
            auto split = header.find(": ");
            if (split == header.npos)
            {
                continue;
            }
            this->customHeaders.emplace(
                std::string_view(header.begin(), header.begin() + split),
                std::string_view(header.begin() + split + 2, header.end()));
        }
    }

    if (this->mainConfig.includes("mime_types") == ValueKind::Array)
    {
        // this->customMimeTypes = true;
    }
}

ServerMode::~ServerMode() {}

void ServerMode::handleFolder(const std::string &host, const fs::path &folderPath, const HTTPPayload &data, HTTPResponse &response, const ReceivedData &client, const std::string_view &ipaddress, const std::string &index)
{
    fs::path filePath = folderPath / decode(data.path);
    FileRead file(filePath);

    if (!file.exists())
    {
        this->logger->info(std::format("{} to {}/{} from {} [404]", data.method, host, data.path, ipaddress));
        response.code = 404;
        response.send(client.fd);
        ::close(client.fd);
        return;
    }

    if (!file.isFolder())
    {
        this->logger->info(std::format("{} to {}/{} from {} [200]", data.method, host, data.path, ipaddress));
        this->doFile(filePath, data, response, file, client.fd);
        ::close(client.fd);
        return;
    }

    fs::path indexPath(folderPath / index);

    if (!fs::exists(indexPath))
    {
        this->logger->info(std::format("{} to {}/{} from {} [404]", data.method, host, std::string(data.path).append(index), ipaddress));
        response.code = 404;
        response.send(client.fd);
        ::close(client.fd);
        return;
    }

    this->logger->info(std::format("{} to {}/{} from {} [200]", data.method, host, std::string(data.path).append(index), ipaddress));
    this->doFile(indexPath, data, response, FileRead(indexPath), client.fd);
    ::close(client.fd);
    return;
}

void ServerMode::handleProxyPass(const HostConfig &hostConfig, HTTPResponse &response, const ReceivedData &client, const HTTPPayload &data, const std::string_view &ipaddress, const std::string &host)
{
    int clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);

    int option = 1;
    if (setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1)
    {
        this->logger->info(std::format("{} to {}/{} from {} [504]", data.method, host, data.path, ipaddress));
        response.code = 504;
        response.send(client.fd);
        ::close(clientSocket);
        return;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(hostConfig.port);
    serverAddress.sin_addr.s_addr = inet_addr(hostConfig.address.data());

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        this->logger->info(std::format("{} to {}/{} from {} [504]", data.method, host, data.path, ipaddress));
        response.code = 504;
        response.send(client.fd);
        ::close(clientSocket);
        return;
    }

    HTTPRequest request(data);
    request.addHeader("X-Forwarded-For", std::string(ipaddress));
    request.addHeader("X-Forwarded-Host", host);
    request.send(clientSocket);

    std::string responseData;
    size_t reserved = 0;
    bool end = false;
    bool sentHeader = false;

    HTTPResponse newResponse("");

    // read headers + some content until response is valid
    // https://stackoverflow.com/a/49823228
    size_t targetContentLength = 0;
    size_t contentSent = 0;
    while (!end)
    {
        constexpr const size_t size = 8 * 1024;
        // 8*1024
        char buffer[size] = {0};
        int readed = static_cast<int>(::recv(clientSocket, &buffer, sizeof(buffer), 0));

        if (readed == -1)
        {
            if (sentHeader == false)
            {
                this->logger->info(std::format("{} to {}/{} from {} [502]", data.method, host, data.path, ipaddress));
                response.code = 502;
                response.send(client.fd);
            }
            ::close(clientSocket);
            return;
        }

        if (readed == 0)
        {
            ::close(clientSocket);
            break;
        }

        if (sentHeader == false)
        {
            ++reserved;
            responseData.reserve(reserved * size);
            responseData.append(std::string{buffer, static_cast<size_t>(readed)});

            newResponse = HTTPResponse(responseData);
            if (newResponse.isValid)
            {
                auto header = newResponse.headers.find(Header{"Content-Length"});

                if (header != newResponse.headers.end())
                {

                    targetContentLength = std::stoul(header->second);
                }

                // add add our headers
                for (auto &header : response.headers)
                {
                    newResponse.headers.emplace(header.first, header.second);
                }

                newResponse.send(client.fd);
                sentHeader = true;
            }

            contentSent += newResponse.content.size();
        }
        else
        {
            if (static_cast<int>(::send(client.fd, buffer, readed, 0)) == -1)
            {
                this->logger->info(std::format("{} to {}/{} from {} [502]", data.method, host, data.path, ipaddress));
                ::close(clientSocket);
                return;
            }

            contentSent += readed;
        }

        // if we sent all data, close and don't read again
        if (contentSent >= targetContentLength && sentHeader == true)
        {
            ::close(clientSocket);
            return;
        }
    }

    this->logger->info(std::format("{} to {}/{} from {} [200]", data.method, host, data.path, ipaddress));
    return;
}

void ServerMode::handleRequest(const ReceivedData &client, const HTTPPayload &data)
{
    std::string_view ipaddress = client.address;
    auto ipForwaredHeader = data.headers.find("X-Forwarded-For");
    if (ipForwaredHeader != data.headers.end())
    {
        ipaddress = ipForwaredHeader->second;
    }

    std::string host;

    auto hostFowardedHeader = data.headers.find("X-Forwarded-Host");
    if (hostFowardedHeader != data.headers.end())
    {
        host = hostFowardedHeader->second;
    }
    else
    {
        auto hostHeader = data.headers.find("Host");
        if (hostHeader == data.headers.end())
        {
            // host is empty
            host = "";
        }
        else
        {
            host = hostHeader->second;
        }
    }

    HTTPResponse response(std::string(data.httpVersion), 200);
    response.headers.emplace("Server", "Tondik/" + Server::version);

    // custom headers
    for (auto &header : this->customHeaders)
    {
        response.headers.emplace(header.first, header.second);
    }

    auto hostConfigPair = this->domainConfigs.find(host);

    if (hostConfigPair == this->domainConfigs.end())
    {
        auto indexFile = this->mainConfig.getValue("index");

        std::string index;

        if (indexFile == std::nullopt)
        {
            index = "index.html";
        }
        else
        {
            index = extractRef(indexFile);
        }

        this->handleFolder(host, extractRef(this->mainConfig.getValue("default_root")),
                           data, response, client, ipaddress, index);
        return;
    }

    auto &hostConfig = hostConfigPair->second;
    auto &hostInit = hostConfig.iniFile;

    if (hostInit.getValue("root") != std::nullopt)
    {
        // handle root

        auto indexFile = hostInit.getValue("index");

        std::string index;

        if (indexFile == std::nullopt)
        {
            index = "index.html";
        }
        else
        {
            index = extractRef(indexFile);
        }

        this->handleFolder(host, extractRef(hostInit.getValue("root")),
                           data, response, client, ipaddress, index);
        return;
    }

    // handle reverse proxy
    this->handleProxyPass(hostConfig, response, client, data, ipaddress, host);

    ::close(client.fd);
}
