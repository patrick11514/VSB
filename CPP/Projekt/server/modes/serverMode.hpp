#pragma once

#include "mainMode.hpp"
#include "../server.hpp"
#include "fstream"

/**
 * @brief Class handling server mode (WIP)
 */
class ServerMode : public MainMode
{
    Socket *socket = nullptr; ///< pointer to socket
    std::ofstream accessLog;
    std::ofstream errorLog;

public:
    /**
     * @brief Constructor
     * @param parser Argument parser
     * @param logger logger
     * @param configPath path to the config directory
     */
    ServerMode(const ArgParser &parser, Logger &logger, const std::string &configPath);
    ~ServerMode() final; ///< destructor
};