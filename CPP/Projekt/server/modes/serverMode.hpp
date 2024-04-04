#pragma once

#include "mainMode.hpp"

class ServerMode : public MainMode
{
public:
    ServerMode(const ArgParser &parser, Logger &logger);
    virtual ~ServerMode();
};