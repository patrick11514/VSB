#include "logger.hpp"

void Logger::info(const std::string &text)
{
    std::cout << "[" << Logger::getTime() << "] [INFO] " << text << "" << std::endl;
}

void Logger::error(const std::string &text)
{
    std::cout << "[" << Logger::getTime() << "] [ERRO] " << text << "" << std::endl;
}

void Logger::warn(const std::string &text)
{
    std::cout << "[" << Logger::getTime() << "] [WARN] " << text << "" << std::endl;
}

std::_Put_time<char> Logger::getTime()
{
    std::time_t t = std::time(nullptr);
    std::tm *tm = std::localtime(&t);
    return std::put_time(tm, "%H:%M:%S");
}
