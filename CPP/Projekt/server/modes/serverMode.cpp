#include "serverMode.hpp"

#include <iostream>
#include <fstream>
#include <format>

ServerMode::ServerMode(const ArgParser &parser, Logger &logger, const std::string &configPath) : MainMode(parser, logger)
{
    fs::path path(fs::canonical(configPath));

    fs::create_directory(path);

    fs::path mainPath(path / "main.ini");

    if (!fs::exists(mainPath))
    {
        std::ofstream configFile(mainPath);
        fs::path accessLog(path / "access.log");
        fs::path errorLog(path / "error.log");
        configFile << std::format("port=80\nconfig_directory=configs\n\naccess_log={}\nerror_log={}", accessLog.string(), errorLog.string());
        configFile.close();
    }

    IniParser config(mainPath);
    if (!config.isOpened())
    {
        throw std::runtime_error("Config file cannot be loaded.");
    }

    if (!config.includes("access_log"))
    {
        throw std::runtime_error("Config doesn't include acces log path");
    }

    if (!config.includes("error_log"))
    {

        throw std::runtime_error("Config doesn't include error log path");
    }

    // cannot be nullptr
    Server *server = Server::instance;

    this->accessLog = std::move(std::ofstream(config.getValue("access_log").value(), std::fstream::app));
    this->errorLog = std::move(std::ofstream(config.getValue("error_log").value(), std::fstream::app));

    Logger *newMainLogger = new Logger(this->accessLog, this->errorLog, this->accessLog);
    // swap pointers and delete old logger
    std::swap(newMainLogger, server->l);
    delete newMainLogger;

    // bind port
    if (!config.includes("port"))
    {
        throw std::runtime_error("Config doesn't include port number");
    }

    int port = std::stoi(config.getValue("port").value());
    if (port < 1 || port > 65353)
    {
        throw std::range_error("Invalid port in range 1-65353");
    }

    this->port = port;

    if (config.includes("local"))
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
}

ServerMode::~ServerMode() {}

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
