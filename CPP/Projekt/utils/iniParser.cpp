#include "iniParser.hpp"
#include <cstring>

void IniParser::skipSpaces(swit &begin, const swit &end) const
{
    while (begin != end)
    {
        if (std::isspace(*begin))
        {
            ++begin;
            continue;
        }
        break;
    }
}

bool IniParser::isComment(swit &begin, const swit &end) const
{
    while (begin != end)
    {
        if (std::isspace(*begin))
        {
            ++begin;
            continue;
        }

        if (*begin == ';')
        {
            return true;
        }
        break;
    }
    return false;
}

std::optional<std::string> IniParser::parseSection(swit &begin, const swit &end, size_t &error) const
{
    if (*begin != '[')
    {
        return std::nullopt;
    }

    std::string_view sw(begin, end);

    auto endBracket = sw.find(']');
    if (endBracket == sw.npos)
    {
        error = 1;
        return std::nullopt;
    }

    std::string_view after(begin + endBracket + 1, end);
    auto afterBegin = after.begin();
    auto afterEnd = after.end();

    this->skipSpaces(afterBegin, afterEnd);
    // characters after ]
    if (afterBegin != afterEnd)
    {
        error = std::distance(begin, afterBegin) + 1;
        return std::nullopt;
    }

    return std::string(begin + 1, begin + endBracket);
}

std::optional<std::pair<std::string_view, std::string_view>> IniParser::parseKeyValue(swit &begin, const swit &end, size_t &error) const
{
    std::string_view sw(begin, end);

    auto separator = sw.find('=');
    if (separator == sw.npos)
    {
        error = 1;
        return std::nullopt;
    }

    return std::make_pair(std::string_view(begin, begin + separator), std::string_view(begin + separator + 1, end));
}

void IniParser::logError(const std::string &filePath, const std::string_view &sw, size_t line, size_t position, const std::string &message)
{
    this->l.error("Unable to parse file: " + filePath);
    this->l.error(std::format("At line {} on {} character", line, position));
    this->l.error(sw);
    std::string str;
    str.reserve(sw.size());
    for (size_t i = 0; i < position - 1; ++i)
    {
        str += " ";
    }
    this->l.error(str + "↑");
    this->l.error(message);
}

std::string IniParser::trim(const std::string_view &str)
{
    auto begin = str.begin();
    auto end = str.end();

    // from start
    while (begin != end)
    {
        if (std::isspace(*begin))
        {
            ++begin;
            continue;
        }
        break;
    }

    --end;

    while (begin != end)
    {
        if (std::isspace(*end))
        {
            --end;
            continue;
        }
        break;
    }

    return std::string(begin, end + 1);
}

IniParser::IniParser(const std::string &filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        return;
    }

    std::string section = "";

    size_t line = 0;

    while (file)
    {
        ++line;
        char buffer[1024];
        file.getline(buffer, sizeof(buffer));

        std::streamsize readed = file.gcount();

        if (readed == 0)
        {
            // eof
            continue;
        }

        size_t size = file.gcount();

        // remove end of string \0
        if (buffer[file.gcount() - 1] == '\0')
        {
            --size;
        }

        if (size == 0)
        {
            continue;
        }

        std::string_view sw(buffer, size);
        auto begin = sw.begin();
        auto end = sw.end();

        this->skipSpaces(begin, end);

        // empty line
        if (begin == end)
        {
            continue;
        }

        if (this->isComment(begin, end))
        {
            continue;
        }

        size_t error = 0;
        auto result = this->parseSection(begin, end, error);

        if (result.has_value())
        {
            section = result.value();
            continue;
        }

        if (error == 1)
        {
            this->logError(filePath, sw, line, sw.size() + 1, "Expected ']' at the end of section");
            exit(-1);
        }

        if (error > 1)
        {
            this->logError(filePath, sw, line, error, "Unexpected symbol after ']'");
            exit(-1);
        }

        auto result2 = this->parseKeyValue(begin, end, error);

        if (result2.has_value())
        {
            auto pair = result2.value();
            std::string key = trim(pair.first);
            std::string value = trim(pair.second);

            std::string fullName(section);
            fullName.reserve(section.size() + key.size() + 1);

            auto it = key.end();

            // check if array
            if (*(it - 2) == '[' && *(it - 1) == ']')
            {
                // remove [] from name
                key.erase(key.end() - 2, key.end());
                fullName += ".";
                fullName += key;

                auto elem = this->keyArrayPairs.find(fullName);
                if (elem == this->keyArrayPairs.end())
                {
                    std::vector<std::string> data;
                    data.emplace_back(value);
                    this->keyArrayPairs.emplace(fullName, data);
                    continue;
                }

                elem->second.emplace_back(value);
                continue;
            }

            if (section.size() > 0)
            {
                fullName += ".";
            }
            fullName += key;

            auto elem = this->keyValuePairs.find(fullName);

            if (elem == this->keyValuePairs.end())
            {
                this->keyValuePairs.emplace(fullName, value);
                continue;
            }

            elem->second = value;
            continue;
        }

        if (error > 0)
        {
            this->logError(filePath, sw, line, sw.size() + 1, "Expected '=' at the end of variable name");
            exit(-1);
        }

        this->l.error("Unable to parse file: " + filePath);
        this->l.error("Cannot parse line " + line);
        exit(-1);
    }

    this->opened = true;
}

IniParser::IniParser(IniParser &&other)
{
    std::swap(this->keyArrayPairs, other.keyArrayPairs);
    std::swap(this->keyValuePairs, other.keyValuePairs);
}

bool IniParser::isOpened() const
{
    return this->opened;
}

ValueKind IniParser::includes(const std::string &key) const
{
    auto it = this->keyValuePairs.find(key);
    if (it != this->keyValuePairs.end())
    {
        return ValueKind::Value;
    }

    auto it2 = this->keyArrayPairs.find(key);
    if (it2 != this->keyArrayPairs.end())
    {
        return ValueKind::Array;
    }

    return ValueKind::Empty;
}

std::optional<std::string> IniParser::getValue(const std::string &key) const
{
    auto it = this->keyValuePairs.find(key);

    if (it == this->keyValuePairs.end())
    {
        return std::nullopt;
    }

    return it->second;
}

std::optional<std::vector<std::string>> IniParser::getArray(const std::string &key) const
{
    auto it = this->keyArrayPairs.find(key);

    if (it == this->keyArrayPairs.end())
    {
        return std::nullopt;
    }

    return it->second;
}