#pragma once

#include <filesystem>
#include <optional>
#include <stdexcept>

namespace fs = std::filesystem;

fs::path decode(const std::string_view &path); ///< decode http encoded path

template <typename T>
using optionalRef = std::optional<std::reference_wrapper<T>>;

template <typename T>
T &extractRef(optionalRef<T> value)
{
    if (!value.has_value()) {
        throw std::runtime_error("No value in optional");
    }

    return value.value().get();
}

/**
 * @brief Structure to hold header name, because we want to ignore case
 */
class Header {
    std::string lowerCaseData; ///< Lowercase name
    std::string originalData;  ///< Original name

    /**
     * @brief Convert string to lowercase
     * @param str Input string
     * @return lower case string
     */
    std::string to_lower(const std::string& str) const;
public:
    Header(const std::string_view& string); ///< Constructor from header
    Header(const Header& other); ///< Copy constructor
    Header(Header&& other); ///< Move constructor

    const std::string& getData() const; ///< Get lowercase name
    const std::string& getOriginal() const; ///< Get original name

    bool operator==(const Header& other) const;
    bool operator!=(const Header& other) const;
};

/**
 * @brief Implement hash function
 */
namespace std {
    template <>
    struct hash<Header>
    {
        std::size_t operator()(const Header& header) const
        {
            return std::hash<std::string>{}(header.getData());
        }
    };
}
