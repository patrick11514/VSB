#include "serverMode.hpp"

ServerMode::ServerMode(const ArgParser &parser, Logger &logger) : MainMode(parser, logger)
{
    std::string programFolder = "/etc/tondik";
    fs::create_directory("/etc/tonda");
}

ServerMode::~ServerMode() {}
