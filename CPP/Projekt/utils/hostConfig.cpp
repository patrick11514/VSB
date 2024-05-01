#include "hostConfig.hpp"
#include "utils.hpp"

HostConfig::HostConfig(IniParser parser): iniFile(std::move(parser)) {
    auto reverseProxy = this->iniFile.getValue("reverse_proxy");
    if (reverseProxy != std::nullopt) {
        std::string address = extractRef(reverseProxy);
        this->hasProxy = true;
        //find colon
        auto colon = address.find(':');
        if (colon == address.npos) {
            this->address = address;
            this->port = 80;
        } else {
            this->address = std::string(address.begin(), address.begin() + colon);
            this->port = std::stoi(std::string(address.begin() + colon + 1, address.end()));
        }
    }
}
