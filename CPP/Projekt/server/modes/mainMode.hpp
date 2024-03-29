#pragma once

#include "../../utils/argParser.hpp"
#include "../../socket/socket.hpp"
#include "../../utils/logger.hpp"
#include "../payload.hpp"

class MainMode
{
    const ArgParser &parser;

protected:
    Logger &logger;

public:
    MainMode(const ArgParser &parser, Logger &logger);
    virtual ~MainMode();

    virtual void handleRequest(const ReceivedData &client, const HTTPPayload &);
};