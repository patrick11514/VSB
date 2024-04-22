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

enum class ValueKind
{
    Empty,
    Value,
    Array
};

/**
 * @brief Simple ini file parser
 */
class IniParser
{
    bool opened = false;                                                     ///< if file is opened
    std::unordered_map<std::string, std::string> keyValuePairs;              ///< key-value pairs
    std::unordered_map<std::string, std::vector<std::string>> keyArrayPairs; ///< key array pairs

    Logger l; ///< logged

    /**
     * @brief Move begin iterator until non space character is found
     * @param begin iterator to begin of the current section
     * @param end iterator to end of the current section
     */
    void skipSpaces(swit &begin, const swit &end) const;
    /**
     * @brief Check if the line is comment
     * @param begin iterator to begin of the current section
     * @param end iterator to end of the current section
     * @return true if line is comment (starts with ';'), otherwise false
     */
    bool isComment(swit &begin, const swit &end) const;
    /**
     * @brief Tries to parse section
     * @param begin iterator to begin of the current section
     * @param end iterator to end of the current section
     * @param error reference to error, if error occures, error code will be written to this reference
     * @return std::string, if section is found ('[SECTION_NAME]'), otherwise std::nullopt
     */
    std::optional<std::string> parseSection(swit &begin, const swit &end, size_t &error) const;
    /**
     * @brief Tries to parse key-value
     * @param begin iterator to begin of the current section
     * @param end iterator to end of the current section
     * @param error reference to error, if error occures, error code will be written to this reference
     * @return pair of string views, if key-value is presented on line ('key=value'), otherwise std::nullopt
     */
    std::optional<std::pair<std::string_view, std::string_view>> parseKeyValue(swit &begin, const swit &end, size_t &error) const;

    /**
     * @brief Log error to logger's error function
     * @param filePath path to the currently parsing file
     * @param sw string view to currently parsed line
     * @param line number of line
     * @param position current position in line
     * @param message error message to be written
     * @example [10:12:57] [ERRO] Unable to parse file: /home/patrick115/Projects/VSB/CPP/Projekt/_testing/main.ini
     * [10:12:57] [ERRO] At line 7 on 9 character
     * [10:12:57] [ERRO] [Section
     * [10:12:57] [ERRO]         ↑
     * [10:12:57] [ERRO] Expected ']' at the end of section
     */
    void logError(const std::string &filePath, const std::string_view &sw, size_t line, size_t position, const std::string &message);

public:
    IniParser() = default;
    IniParser(const std::string &filePath); ///< constructor
    IniParser(IniParser&&other);

    bool isOpened() const;                                                          ///< check if file is opened
    ValueKind includes(const std::string &key) const;                               ///< check if key is presented in ini file
    std::optional<std::string> getValue(const std::string &key) const;              ///< get value from ini file by key
    std::optional<std::vector<std::string>> getArray(const std::string &key) const; ///< get array from ini file by key

    static std::string trim(const std::string_view &str); ///< trim string
};