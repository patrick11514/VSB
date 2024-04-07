#pragma once

#include <filesystem>

#include "../../utils/argParser.hpp"
#include "../../utils/logger.hpp"
#include "../../utils/fileRead.hpp"
#include "../payload.hpp"
#include "../response.hpp"

namespace fs = std::filesystem;

/**
 * @brief Main server mode
 */
class MainMode
{
    const ArgParser &parser; ///< argument parser

protected:
    Logger &logger; ///< logger

    /**
     * @brief Open file and serve it to client
     * @param filePath path to the file
     * @param HTTPPayload http payload from client
     * @param response response to the client
     * @param file information about file
     * @param fd file descriptor for connection with client
     */
    void doFile(const fs::path &filePath, const HTTPPayload &payload, HTTPResponse &response, const FileRead &file, int fd) const;

public:
    MainMode(const ArgParser &parser, Logger &logger); ///< constructor
    virtual ~MainMode();                               ///< virtual destructor

    virtual void handleRequest(const ReceivedData &client, const HTTPPayload &); ///< handle request (called from server.cpp)
};