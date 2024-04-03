#pragma once

#include <filesystem>

#include "mainMode.hpp"

class DevMode : public MainMode
{
    fs::path path;
    std::vector<std::string> indexes;

public:
    DevMode(const ArgParser &parser, Logger &logger);

    void handleRequest(const ReceivedData &client, const HTTPPayload &data) final;
};