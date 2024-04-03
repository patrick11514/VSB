#include "iniParser.hpp"

#include <iostream>

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

        // remove end of string \0
        size_t size = file.gcount() - 1;

        if (size == 0)
        {
            continue;
        }

        std::string_view sw(buffer, size);

        // empty line
        bool empty = true;
        for (auto it = sw.begin(); it != sw.end(); ++it)
        {
            if (!std::isspace(*it))
            {
                empty = false;
                break;
            }
        }

        if (empty)
        {
            continue;
        }

        // comment
        bool comment = false;
        for (auto it = sw.begin(); it != sw.end(); ++it)
        {
            if (std::isspace(*it))
            {
                continue;
            }

            if (*it == ';')
            {
                comment = true;
                break;
            }

            break;
        }

        std::cout << sw << "||"
                  << "c: " << comment << std::endl;

        if (comment)
        {
            continue;
        }

        if (sw[0] == '[')
        {
            // section?
            auto end = sw.find(']');
            if (end == sw.npos)
            {
                this->logError(filePath, sw, line, sw.size() + 1, "Expected ']' at the end of section");
                exit(-1);
            }

            section = std::string(sw.begin() + 1, sw.begin() + end);
            continue;
        }

        auto separator = sw.find('=');

        if (separator == sw.npos)
        {
            this->logError(filePath, sw, line, sw.size() + 1, "Expected '=' at the end of variable name");
            exit(-1);
        }

        size_t begin = 0;
        for (auto it = sw.begin(); it != sw.end(); ++it)
        {
            if (std::isspace(*it))
            {
                ++begin;
                continue;
            }
            break;
        }

        std::string_view name(sw.begin() + begin, sw.begin() + separator);
        std::string_view value(sw.begin() + separator + 1, sw.end());
    }
}

bool IniParser::isOpened() const
{
    return this->opened;
}
