#pragma once

#include <string>
#include <cstdint>
#include <functional>
#include <iostream>
#include <utility>
#include <cstring>
#include <optional>

using CodePoint = uint32_t;

/**
 * @brief UTF8 strings
 */
class UTF8String
{
private:
    int size = 0;             ///< Size of bytes in data
    int capacity = 0;         ///< Capacity of data
    uint16_t *data = nullptr; ///< Stored bytes

    /**
     * @brief Get closes power of two
     *
     * @param i Number which should by lower than the power of two which we are finding
     * @return Power of two bigger than i
     */
    int _getClosestPowerOfTwo(int x) const;
    /**
     * @brief Decode code point to vector of bytes
     *
     * @param cp Code point
     * @return Vector of bytes
     */
    std::vector<uint16_t> _decodeCodePoint(CodePoint cp) const;
    /**
     * @brief Decode vector of code points to vector of bytes
     *
     * @param input Vector of code points
     * @return Vector of bytes
     */
    std::vector<uint16_t> _decodeCodePoints(std::vector<CodePoint> cp) const;
    /**
     * @brief Encode internal data to vector of code points
     *
     * @return Vector of code points
     */
    std::vector<CodePoint> _encodeCodePoints() const;

    /**
     * @brief Create mask for shifting first part of code point
     * @param ones Number of ones + 1 in mask
     * @return Final mask
     */
    int _createMask(int ones) const;

    /**
     * @brief Resizes the buffer twices
     * @param minSize You can provide a minSize number, and buffer should be large enough, to append store minSize new bytes
     */
    void _resizeBuffer(int minSize);

public:
    UTF8String() = default; ///< Default constructor
    /**
     * @brief Construtor from string
     * @param str String from which will be UTF8String constructed
     */
    UTF8String(std::string str);
    /**
     * @brief Construct from vector of CodePoints
     * @param cps Vector of CodePoints
     */
    UTF8String(std::vector<CodePoint> cps);
    UTF8String(const UTF8String &instance); ///< Copy constructor
    ~UTF8String();                          ///< Destructor

    /**
     * @brief Append new char to UTF8String
     * @param c Character to be appended
     */
    void append(char c);
    /**
     * @brief Append new CodePoint to UTF8String
     * @param cp CodePoint to be appended
     */
    void append(CodePoint cp);

    /**
     * @brief Get count of bytes
     * @return Count of bytes in UTF8String
     */
    int get_byte_count() const;
    /**
     * @brief Get number of Codepoints
     * @return Number of CodePoints in UTF8String
     */
    int get_point_count() const;
    /**
     * @brief Return n-th CodePoint in UTF8String
     * @param index Index of CodePoint
     * @return CodePoint on index or std::nullopt
     */
    std::optional<CodePoint> nth_code_point(const int &index) const;

    /**
     * @brief Compare operator, UTF8Strings equals if bytes are same
     * @param right UTF8String to compare
     * @return true if bytes of UTF8Strings are same otherwise false
     */
    bool operator==(const UTF8String &right) const;
    bool operator!=(const UTF8String &right) const; ///< != operator, negation of == operator
    /**
     * @brief Operator to get specific byte from UTF8String
     * @param index Index of byte
     * @return byte if index is in range, otherwite std::nullopt
     */
    std::optional<uint16_t> operator[](const int &index) const;
    UTF8String operator+(const UTF8String &right) const; ///< + operator to concatenate UTF8Strings
    operator std::string() const;                        ///< Cast UTF8String to std::string

    UTF8String &operator=(const UTF8String &right);  ///< Assing operator
    UTF8String &operator+=(const UTF8String &right); ///< append UTF8String
};
