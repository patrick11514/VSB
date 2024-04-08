#pragma once

#include <filesystem>

#include "mainMode.hpp"

/**
 * @brief Class handling devmode
 */
class DevMode : public MainMode
{
    fs::path path;                      ///< path to the served directory from --path arg or ./
    std::array<std::string, 2> indexes; ///< list of index files (index.html, index.htm)

public:
    DevMode(const ArgParser &parser, Logger &logger); ///< constructor
    ~DevMode() final;                                 ///< destructor

    void handleRequest(const ReceivedData &client, const HTTPPayload &data) final; ///< handle request
};