
#include "iniParser.hpp"
#include "../server/server.hpp"

// Helper struct
struct HostConfig
{
    IniParser iniFile;
    std::string address;
    int port;

    bool hasProxy = false;

    HostConfig(IniParser parser);
};
