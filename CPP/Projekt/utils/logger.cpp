#include "logger.hpp"

Logger::Logger(std::ostream &infoStream, std::ostream &errorStream, std::ostream &warnStream)
    : infoStream(infoStream), errorStream(errorStream), warnStream(warnStream) {}

Logger::~Logger()
{
    // sync streams
    this->infoStream.flush();
    this->errorStream.flush();
    this->warnStream.flush();
}

void Logger::info(const std::string_view &text)
{
    std::lock_guard<std::mutex> lock(Logger::mutex);

    this->infoStream << "[" << this->getTime() << "] [INFO] " << text << "\n";
}

void Logger::error(const std::string_view &text)
{
    std::lock_guard<std::mutex> lock(Logger::mutex);
    this->errorStream << "[" << Logger::getTime() << "] [ERRO] " << text << "\n";
}

void Logger::warn(const std::string_view &text)
{
    std::lock_guard<std::mutex> lock(Logger::mutex);
    this->warnStream << "[" << Logger::getTime() << "] [WARN] " << text << "\n";
}
std::_Put_time<char> Logger::getTime()
{
    std::time_t t = std::time(nullptr);
    std::tm *tm = std::localtime(&t);
    return std::put_time(tm, "%H:%M:%S");
}
