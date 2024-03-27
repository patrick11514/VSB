#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <format>

/**
 * @brief Basic logged to console
 */
struct Logger
{
    static void info(const std::string &text);  ///< [TIME] [INFO] text
    static void error(const std::string &text); ///< [TIME] [ERRO] text
    static void warn(const std::string &text);  ///< [TIME] [WARN] text

private:
    static std::_Put_time<char> getTime(); ///< get current formatted time HH:MM:SS
};