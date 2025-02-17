#pragma once

#include <unordered_map>
#include <string>

/**
 * @brief Parse arguments from program
 */
class ArgParser
{
    std::string filePath;                                ///< Path to current program
    std::unordered_map<std::string, std::string> values; ///< HashMap of key value pairs of arguments

public:
    ArgParser(int argc, char **argv); ///< constructor

    /**
     * @brief Check if arguments includes key
     * @param key Key to check
     * @return true if inclues otherwise false
     */
    bool includes(const std::string &key) const;
    /**
     * @brief Get value of argument by key
     * @param key Key of value
     * @return nullptr if not found, otherwise ptr to constant value
     */
    const std::string *getByKey(const std::string &key) const;
};