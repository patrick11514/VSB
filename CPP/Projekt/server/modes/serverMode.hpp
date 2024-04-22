#pragma once

#include "mainMode.hpp"
#include "../server.hpp"
#include "fstream"
#include "../../utils/iniParser.hpp"

/**
 * @brief Class handling server mode (WIP)
 */
class ServerMode : public MainMode
{
    Socket *socket = nullptr;                                 ///< pointer to socket
    std::ofstream accessLog;                                  ///< access log output file stream
    std::ofstream errorLog;                                   ///< error log output file stream
    std::unordered_map<std::string, IniParser> domainConfigs; ///< configs for domains

    void createDefaultHTMLFile(const fs::path &folderPath, const fs::path &mainConfig, const fs::path &domainConfigs) const;

public:
    bool local = false; ///< If server is opened only on local network
    int port;           ///< Port number
    /**
     * @brief Constructor
     * @param parser Argument parser
     * @param logger logger
     * @param configPath path to the config directory
     */
    ServerMode(const ArgParser &parser, Logger &logger, const std::string &configPath);
    ~ServerMode() final; ///< destructor

    void handleRequest(const ReceivedData &client, const HTTPPayload &data) final; ///< handle request
};