#include "serverMode.hpp"
#include "../../utils/iniParser.hpp"

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
        configFile << std::format("port=80\n\naccess_log={}\nerror_log={}", accessLog.string(), errorLog.string());
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

    this->accessLog = std::move(std::ofstream(config.getValue("access_log").value()));
    this->errorLog = std::move(std::ofstream(config.getValue("error_log").value()));

    Logger *newMainLogger = new Logger(this->accessLog, this->errorLog, this->accessLog);
    // swap pointers and delete old logger
    std::swap(newMainLogger, server->l);
    delete newMainLogger;
}

ServerMode::~ServerMode() {}
