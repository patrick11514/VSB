
//Helper struct
#include "iniParser.hpp"
struct HostConfig {
    IniParser iniFile;
    std::string address;
    int port;

    bool hasProxy = false;

    HostConfig(IniParser parser);
};
