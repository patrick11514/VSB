#include "argParser.hpp"

ArgParser::ArgParser(int argc, char **argv) : filePath(argv[0])
{
    const char *key = nullptr;
    for (int i = 1; i < argc; ++i)
    {
        std::string current(argv[i]);
        if (current.starts_with("--"))
        {
            std::string_view sw(current.begin() + 2, current.end());
            size_t result = sw.find('=');
            if (result == std::string::npos)
            {
                this->values.emplace(sw, "");
                continue;
            }

            std::string_view key(sw.begin(), sw.begin() + result);
            std::string_view value(sw.begin() + result + 1, sw.end());
            this->values.emplace(key, value);
            continue;
        }
        else if (current.starts_with("-"))
        {
            if (key != nullptr)
            {
                this->values.emplace(key + 1, "");
            }

            key = argv[i];
            continue;
        }
        if (key != nullptr)
        {
            this->values.emplace(key + 1, current);
            key = nullptr;
        }
    }

    if (key != nullptr)
    {
        this->values.emplace(key + 1, "");
    }
}

bool ArgParser::includes(const std::string &key) const
{
    return this->values.contains(key);
}

const std::string *ArgParser::getByKey(const std::string &key) const
{
    auto it = this->values.find(key);
    if (it == this->values.end())
    {
        return nullptr;
    }
    return &it->second;
}
