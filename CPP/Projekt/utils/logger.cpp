#include "logger.hpp"

void Logger::info(const std::string &text)
{
    std::cout << "[" << Logger::getTime() << "] [INFO] " << text << "\n";
}

void Logger::error(const std::string &text)
{
    std::cout << "[" << Logger::getTime() << "] [ERRO] " << text << "\n";
}

void Logger::warn(const std::string &text)
{
    std::cout << "[" << Logger::getTime() << "] [WARN] " << text << "\n";
}

std::_Put_time<char> Logger::getTime()
{
    std::time_t t = std::time(nullptr);
    std::tm *tm = std::localtime(&t);
    return std::put_time(tm, "%H:%M:%S");
}
