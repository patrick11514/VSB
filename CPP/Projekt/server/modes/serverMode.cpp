#include "serverMode.hpp"

#include <iostream>
#include <fstream>
#include <format>

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

ServerMode::ServerMode(const ArgParser &parser, Logger &logger, const std::string &configPath) : MainMode(parser, logger)
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

    IniParser config(mainPath);
    if (!config.isOpened())
    {
        throw std::runtime_error("Config file cannot be loaded.");
    }

    if (config.includes("access_log") != ValueKind::Value)
    {
        throw std::runtime_error("Config doesn't include acces log path");
    }

    if (config.includes("error_log") != ValueKind::Value)
    {

        throw std::runtime_error("Config doesn't include error log path");
    }

    if (config.includes("default_root") != ValueKind::Value)
    {
        throw std::runtime_error("Config doesn't include default root");
    }

    if (config.includes("config_directory") != ValueKind::Value)
    {
        throw std::runtime_error("Config doesn't include configs directory");
    }

    // if doesn't exists, create it
    fs::path defaultRoot(config.getValue("default_root").value());
    if (!fs::exists(defaultRoot))
    {
        createDefaultHTMLFile(config.getValue("default_root").value(), mainPath, config.getValue("config_directory").value());
    }

    // cannot be nullptr
    Server *server = Server::instance;

    // bind port
    if (config.includes("port") != ValueKind::Value)
    {
        throw std::runtime_error("Config doesn't include port number");
    }

    int port = std::stoi(config.getValue("port").value());
    if (port < 1 || port > 65353)
    {
        throw std::range_error("Invalid port in range 1-65353");
    }

    this->port = port;

    if (config.includes("local") == ValueKind::Value)
    {
        auto value = config.getValue("local").value();
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
    auto configDirectory = config.getValue("config_directory");
    if (!configDirectory.has_value())
    {
        throw std::runtime_error("Config doesn't include config directory");
    }

    fs::path configsPath(path / configDirectory.value());
    std::cout << configsPath << std::endl;

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

        this->domainConfigs.emplace(configParser.getValue("domain").value(), std::move(configParser));
    }

    // swap logger at the end
    // because we store original streams in ServerMode and we are passing referenes to Logger
    // if this constructor throws error, references will be invalid, and error cannot be written into
    // file
    this->accessLog = std::move(std::ofstream(config.getValue("access_log").value(), std::fstream::app));
    this->errorLog = std::move(std::ofstream(config.getValue("error_log").value(), std::fstream::app));

    Logger *newMainLogger = new Logger(this->accessLog, this->errorLog, this->accessLog);
    // swap pointers and delete old logger
    std::swap(newMainLogger, server->l);
    delete newMainLogger;

    server->l->info(std::format("Loaded {} domain config files", this->domainConfigs.size()));
}

ServerMode::~ServerMode()
{
    std::cout << "called" << std::endl;
}

void ServerMode::handleRequest(const ReceivedData &client, const HTTPPayload &data)
{
    for (auto header : data.headers)
    {
        std::cout << header.first << ": " << header.second << std::endl;
    }

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

    std::cout << ipaddress << " -> " << host << std::endl;

    ::close(client.fd);
}
