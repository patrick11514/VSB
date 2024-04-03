#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <vector>

#include "logger.hpp"

namespace fs = std::filesystem;

class IniParser
{
    bool opened = false;
    std::unordered_map<std::string, std::string> keyValuePairs;
    std::unordered_map<std::string_view, std::vector<std::string>> keyArrayPairs;

    Logger l;

    void logError(const std::string &filePath, const std::string_view &sw, size_t line, size_t position, const std::string &message);

public:
    IniParser(const std::string &filePath);

    bool isOpened() const;
};