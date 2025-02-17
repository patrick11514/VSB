#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

std::vector<char> genChars(int count)
{
    std::vector<char> chars;
    chars.reserve(count);

    for (int i = 0; i < count; ++i)
    {
        chars.emplace_back('a' + i);
    }

    return chars;
}

std::vector<std::string> createIfs(std::vector<char> chars, size_t depth);

void intend(std::vector<std::string> &lines, std::vector<char> chars, size_t depth)
{
    std::vector<std::string> nestedLines = createIfs(chars, depth - 1);

    std::transform(nestedLines.begin(), nestedLines.end(), std::back_inserter(lines), [](const std::string &line)
                   { return std::string{"  " + line}; });
}

std::vector<std::string> createIfs(std::vector<char> chars, size_t depth)
{
    if (depth == 1)
    {
        std::string str{"writeln("};
        for (size_t i = 0; i < chars.size(); ++i)
        {
            if (i > 0)
            {
                str.push_back(',');
            }
            str.push_back(chars[i]);
        }
        str.push_back(')');

        return {str};
    }

    int firstIndex = chars.size() - depth;
    int secondIndex = firstIndex + 1;

    std::vector<std::string> lines;
    std::stringstream line;
    // IF
    line << "if " << chars[firstIndex] << " < " << chars[secondIndex] << " then";
    lines.push_back(line.str());
    intend(lines, chars, depth);

    for (size_t i = 0; i < chars.size() - depth; ++i)
    {
        // ELSE IF
        std::swap(chars[firstIndex], chars[secondIndex]);
        // move indexes
        --firstIndex;
        --secondIndex;
        // print
        line.str(std::string());
        line << "else if " << chars[firstIndex] << " < " << chars[secondIndex] << " then";
        lines.push_back(line.str());
        intend(lines, chars, depth);
    }

    // ELSE
    std::swap(chars[firstIndex], chars[secondIndex]);
    lines.push_back("else");

    intend(lines, chars, depth);

    return lines;
}

void createProgram(int varCount)
{
    std::cout << "program sort(input,output);\nvar\n";

    std::vector<char> chars = genChars(varCount);
    std::string joinedChars;
    joinedChars.reserve(varCount * 2 - 1);

    for (size_t i = 0; i < chars.size(); ++i)
    {
        if (i > 0)
        {
            joinedChars.push_back(',');
        }
        joinedChars.push_back(chars[i]);
    }

    std::cout << joinedChars << " : integer;\nbegin\n  readln(" << joinedChars << ");\n";

    // generate body
    std::vector<std::string> lines = createIfs(chars, chars.size());

    for (const auto &line : lines)
    {
        std::cout << "  " << line << "\n";
    }

    std::cout << "end." << std::endl;
}

int main()
{
    int programs;
    std::cin >> programs;

    for (int i = 0; i < programs; ++i)
    {
        if (i > 0)
        {
            std::endl(std::cout);
        }

        int varCount;
        std::cin >> varCount;

        createProgram(varCount);
    }

    return 0;
}
