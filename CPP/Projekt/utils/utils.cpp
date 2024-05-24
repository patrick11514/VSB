#include <iostream>
#include <sstream>
#include <algorithm>

#include "utils.hpp"

bool isValid(char c)
{
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

fs::path decode(const std::string_view &path)
{
    std::string newPath;
    newPath.reserve(path.size());

    bool isChar = false;

    for (auto it = path.begin(); it != path.end(); ++it)
    {
        auto c = *it;

        if (isChar == true)
        {
            char c1 = *it;
            char c2 = *(it + 1);

            if (!isValid(c1) || !isValid(c2))
            {
                // reset and process it as normal char
                newPath += "%";
                isChar = false;
                it--;
                continue;
            }

            std::string chars(it, it + 2);

            newPath += static_cast<char>(std::stol(chars, nullptr, 16));

            it++;
            isChar = false;

            continue;
        }

        if (c == '%')
        {
            isChar = true;
            continue;
        }

        newPath += c;
    }

    return newPath;
}

std::string Header::to_lower(const std::string &str) const
{
    std::string newStr{str};
    std::transform(newStr.begin(), newStr.end(), newStr.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });

    return newStr;
}

Header::Header(const std::string_view &str) : lowerCaseData(to_lower(std::string{str})), originalData(str) {}
Header::Header(const Header &other) : lowerCaseData(other.lowerCaseData), originalData(other.originalData) {}
Header::Header(Header &&other)
{
    std::swap(this->lowerCaseData, other.lowerCaseData);
    std::swap(this->originalData, other.originalData);
}

const std::string &Header::getData() const
{
    return this->lowerCaseData;
}

const std::string &Header::getOriginal() const
{
    return this->originalData;
}

bool Header::operator==(const Header &other) const
{
    return this->lowerCaseData == other.lowerCaseData;
}

bool Header::operator!=(const Header &other) const
{
    return !this->operator==(other);
}

DomainLogger::DomainLogger(const std::string &accessPath, const std::string &errorPath)
    : accessLog(accessPath, std::ios::app), errorLog(errorPath, std::ios::app)
{
    this->logger = new Logger(this->accessLog, this->errorLog, this->accessLog);
}

DomainLogger::DomainLogger(DomainLogger &&other)
{
    // we cannot swap loggers, because in swapped logger will be invalid references, so we delete current logger
    delete this->logger;

    std::swap(this->accessLog, other.accessLog);
    std::swap(this->errorLog, other.errorLog);

    // create new and old logger in other will be deleted with destructor
    this->logger = new Logger(this->accessLog, this->errorLog, this->accessLog);
}

DomainLogger::~DomainLogger()
{
    delete this->logger;
}
