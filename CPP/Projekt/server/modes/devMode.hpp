#pragma once

#include <filesystem>

#include "mainMode.hpp"
#include "../response.hpp"
#include "../../utils/fileRead.hpp"
#include "../../utils/decode.hpp"

namespace fs = std::filesystem;

class DevMode : public MainMode
{
    fs::path path;
    std::vector<std::string> indexes;

    void doFile(const fs::path &filePath, const HTTPPayload &payload, HTTPResponse &response, const FileRead &file, int fd) const;

public:
    DevMode(const ArgParser &parser, Logger &logger);

    void handleRequest(const ReceivedData &client, const HTTPPayload &data) final;
};