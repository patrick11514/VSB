#pragma once

#include "mainMode.hpp"
#include "fstream"
#include "../../utils/iniParser.hpp"
#include "../../utils/hostConfig.hpp"

/**
 * @brief Class handling server mode (WIP)
 */
class ServerMode final : public MainMode
{
    Socket *socket = nullptr;                                    ///< pointer to socket
    std::ofstream accessLog;                                     ///< access log output file stream
    std::ofstream errorLog;                                      ///< error log output file stream
    IniParser mainConfig;                                        ///< Main.ini config
    std::unordered_map<std::string, HostConfig> domainConfigs;   ///< configs for domains
    std::unordered_map<std::string, DomainLogger> domainLoggers; ///< loggers for domains

    void createDefaultHTMLFile(const fs::path &folderPath, const fs::path &mainConfig, const fs::path &domainConfigs) const; ///< Create default html file, server as index, if no config was found

    std::unordered_map<std::string_view, std::string_view> customHeaders; ///< Map of custom headers from main config

    /**
     * @brief Handle folder for specific config
     * @param host Host name to be printed in log
     * @param folderPath Path to the root of host
     * @param payload HTTP payload
     * @param response HTTP response which will be sent
     * @param client Info about client
     * @param ipaddress Correct ip address of client
     * @param index index file from config
     * @param logger logger used by host
     */
    void handleFolder(const std::string &host, const fs::path &folderPath, const HTTPPayload &payload, HTTPResponse &response, const ReceivedData &client, const std::string_view &ipaddress, const std::string &index, Logger *logger);
    /**
     * @brief Handle proxy pass connection
     * @param hostConfig Config of accessed host
     * @param response Response to be send
     * @param client Info about client
     * @param data HTTP Payload
     * @param ipaddress Correct ip address of client
     * @param host Host name to be printed in log
     * @param logger logger used by host
     */
    void handleProxyPass(const HostConfig &hostConfig, HTTPResponse &response, const ReceivedData &client, const HTTPPayload &data, const std::string_view &ipaddress, const std::string &host, Logger *logger);

public:
    bool local = false; ///< If server is opened only on local network
    int port;           ///< Port number
    /**
     * @brief Constructor
     * @param parser Argument parser
     * @param logger logger
     * @param configPath path to the config directory
     */
    ServerMode(const ArgParser &parser, Logger *logger, const std::string &configPath);
    ~ServerMode() final; ///< destructor

    void handleRequest(const ReceivedData &client, const HTTPPayload &data) final; ///< handle request
};
