#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <vector>

#include "logger.hpp"

namespace fs = std::filesystem;

using swit = std::string_view::iterator;

class IniParser
{
    bool opened = false;
    std::unordered_map<std::string, std::string> keyValuePairs;
    std::unordered_map<std::string, std::vector<std::string>> keyArrayPairs;

    Logger l;

    void skipSpaces(swit &begin, const swit &end) const;
    bool isComment(swit &begin, const swit &end) const;
    std::optional<std::string> parseSection(swit &begin, const swit &end, size_t &error) const;
    std::optional<std::pair<std::string_view, std::string_view>> parseKeyValue(swit &begin, const swit &end, size_t &error) const;

    void logError(const std::string &filePath, const std::string_view &sw, size_t line, size_t position, const std::string &message);

public:
    IniParser(const std::string &filePath);

    bool isOpened() const;
    bool includes(const std::string &key) const;
    std::optional<std::string> getValue(const std::string &key) const;
    std::optional<std::vector<std::string>> getArray(const std::string &key) const;

    static std::string trim(const std::string_view &str);
};