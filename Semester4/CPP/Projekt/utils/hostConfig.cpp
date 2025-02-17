#include "hostConfig.hpp"
#include "utils.hpp"

HostConfig::HostConfig(IniParser parser) : iniFile(std::move(parser))
{
    auto reverseProxy = this->iniFile.getValue("reverse_proxy");
    if (reverseProxy != std::nullopt)
    {
        std::string address = extractRef(reverseProxy);
        this->hasProxy = true;
        // find colon
        auto colon = address.find(':');
        if (colon == address.npos)
        {
            this->address = address;
            this->port = 80;
        }
        else
        {
            this->address = std::string(address.begin(), address.begin() + colon);
            this->port = std::stoi(std::string(address.begin() + colon + 1, address.end()));
        }
    }
    /*
        // setup logger
        auto accessLog = this->iniFile.getValue("access_log");
        auto errorLog = this->iniFile.getValue("error_log");

        // both options must be set
        if (accessLog.has_value() && errorLog.has_value())
        {
            auto accessLogPath = extractRef(accessLog);
            auto errorLogPath = extractRef(errorLog);

            this->accessLog = new std::ofstream(accessLogPath, std::fstream::app);
            this->errorLog = new std::ofstream(errorLogPath, std::fstream::app);

            this->logger = new Logger(*this->accessLog, *this->errorLog, *this->accessLog);
            return;
        }
        */

    // leave logger to nullptr and main logger will be used
}