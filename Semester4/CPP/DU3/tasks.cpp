#include "tasks.h"

// TODO: implement functions from tasks.h

Parser create_char_parser(const char c)
{
    return [c](const char *str) -> const char *
    {
        size_t len = strlen(str);
        if (len == 0)
        {
            return nullptr;
        }

        if (*str == c)
        {
            return str + 1;
        }

        return nullptr;
    };
}

Parser regex_match(const char *regexString)
{
    std::regex regex(regexString);
    return [regex](const char *str) -> const char *
    {
        std::cmatch match;
        bool matched = std::regex_search(str, match, regex);
        if (!matched)
        {
            return nullptr;
        }

        if (match.prefix().length() > 0)
        {
            return nullptr;
        }

        size_t end = 0;

        for (size_t i = 0; i < match.size(); ++i)
        {
            end += match[i].length();
        }

        return str + end;
    };
}

Parser skip_ws()
{
    return [](const char *str) -> const char *
    {
        size_t size = strlen(str);

        for (size_t i = 0; i < size; ++i)
        {
            char c = str[i];
            if (std::isspace(c))
            {
                continue;
            }

            return str + i;
        }

        return str;
    };
}

Parser any_of(const std::vector<Parser> &parsers)
{
    return [parsers](const char *str) -> const char *
    {
        for (size_t i = 0; i < parsers.size(); ++i)
        {
            auto result = parsers[i](str);
            if (result != nullptr)
            {
                return result;
            }
        }

        return nullptr;
    };
}

Parser sequence(const std::vector<Parser> &parsers)
{
    return [parsers](const char *str) -> const char *
    {
        for (size_t i = 0; i < parsers.size(); ++i)
        {
            str = parsers[i](str);
            if (str == nullptr)
            {
                return nullptr;
            }
        }

        return str;
    };
}

Parser repeat(const Parser &parser, const size_t repetitions)
{
    return [parser, repetitions](const char *str) -> const char *
    {
        for (size_t i = 0; i < repetitions; ++i)
        {
            str = parser(str);
            if (str == nullptr)
            {
                return nullptr;
            }
        }

        return str;
    };
}

Parser create_word_parser(const std::string_view &string)
{
    return [string](const char *str) -> const char *
    {
        std::vector<Parser> parsers;
        parsers.reserve(string.size());

        for (size_t i = 0; i < string.size(); ++i)
        {
            parsers.push_back(create_char_parser(string[i]));
        }

        return sequence(parsers)(str);
    };
}
