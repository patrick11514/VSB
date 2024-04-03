#pragma once

#include <filesystem>

#include "../../utils/argParser.hpp"
#include "../../socket/socket.hpp"
#include "../../utils/logger.hpp"
#include "../payload.hpp"
#include "../../utils/fileRead.hpp"
#include "../../utils/decode.hpp"
#include "../response.hpp"

namespace fs = std::filesystem;

class MainMode
{
    const ArgParser &parser;

protected:
    Logger &logger;

    void doFile(const fs::path &filePath, const HTTPPayload &payload, HTTPResponse &response, const FileRead &file, int fd) const;

public:
    MainMode(const ArgParser &parser, Logger &logger);
    virtual ~MainMode();

    virtual void handleRequest(const ReceivedData &client, const HTTPPayload &);
};