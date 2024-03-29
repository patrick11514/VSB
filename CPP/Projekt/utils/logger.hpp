#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <format>
#include <mutex>

/**
 * @brief Basic logged to console
 */
class Logger
{
    std::_Put_time<char> getTime(); ///< get current formatted time HH:MM:SS
    std::mutex mutex;
    std::ostream &infoStream = std::cout;
    std::ostream &errorStream = std::cerr;
    std::ostream &warnStream = std::cout;

public:
    Logger() = default;
    Logger(std::ostream &infoStream, std::ostream &errorStream, std::ostream &warnStream);

    void info(const std::string &text);  ///< [TIME] [INFO] text
    void error(const std::string &text); ///< [TIME] [ERRO] text
    void warn(const std::string &text);  ///< [TIME] [WARN] text
};