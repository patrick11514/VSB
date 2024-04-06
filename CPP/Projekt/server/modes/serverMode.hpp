#pragma once

#include "mainMode.hpp"

class ServerMode : public MainMode
{
    Socket *socket = nullptr;

public:
    ServerMode(const ArgParser &parser, Logger &logger, const std::string &configPath);
    virtual ~ServerMode();
};