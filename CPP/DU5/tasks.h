#pragma once

#include <memory>
#include <cstdint>
#include <string>
#include <functional>
#include <iostream>
#include <utility>
#include <cstring>
#include <optional>

class BytesIterator
{
    const uint8_t *data;
    size_t size;

public:
    BytesIterator(const uint8_t *bytes, size_t size);

    BytesIterator begin() const;
    BytesIterator end() const;

    uint8_t operator*() const;
    bool operator==(const BytesIterator &other) const;
    bool operator!=(const BytesIterator &other) const;
    BytesIterator &operator++();
    BytesIterator operator++(int);
    BytesIterator &operator--();
    BytesIterator operator--(int);
    BytesIterator operator+=(size_t value);
    BytesIterator operator-=(size_t value);
    BytesIterator operator+(size_t value);
    BytesIterator operator-(size_t value);
};

using CodePoint = uint32_t;

class CodePointIterator
{
    const uint8_t *data;
    size_t size;

    CodePoint _getCodePoint() const;
    size_t _getCodePointEnd() const;

public:
    CodePointIterator(const uint8_t *bytes, size_t size);

    CodePointIterator begin() const;
    CodePointIterator end() const;

    CodePoint operator*() const;
    bool operator==(const CodePointIterator &other) const;
    bool operator!=(const CodePointIterator &other) const;
    CodePointIterator &operator++();
    CodePointIterator operator++(int);
    CodePointIterator &operator--();
    CodePointIterator operator--(int);
};

/// UTF-8 string (reuse your previous implementation and modify it)

/**
 * @brief UTF8 strings
 */
class UTF8String
{
private:
    std::vector<uint8_t> data; ///< Stored bytes

    /**
     * @brief Decode code point to vector of bytes
     *
     * @param cp Code point
     * @return Vector of bytes
     */
    std::vector<uint8_t> _encodeCodePoint(CodePoint cp) const;
    /**
     * @brief Decode vector of code points to vector of bytes
     *
     * @param input Vector of code points
     * @return Vector of bytes
     */
    std::vector<uint8_t> _encodeCodePoints(std::vector<CodePoint> cps) const;
    /**
     * @brief Encode internal data to vector of code points
     *
     * @return Vector of code points
     */
    std::vector<CodePoint> _decodeCodePoints() const;

    /**
     * @brief Create mask for shifting first part of code point
     * @param ones Number of ones + 1 in mask
     * @return Final mask
     */
    int _createMask(int ones) const;

public:
    UTF8String() = default; ///< Default constructor
    /**
     * @brief Construtor from string
     * @param str String from which will be UTF8String constructed
     */
    UTF8String(const std::string &str);
    /**
     * @brief Construct from vector of CodePoints
     * @param cps Vector of CodePoints
     */
    UTF8String(const std::vector<CodePoint> &cps);
    UTF8String(const std::vector<uint8_t> &bytes);
    UTF8String(const UTF8String &instance); ///< Copy constructor
    UTF8String(UTF8String &&instance);      ///< Move constructor

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
    size_t get_byte_count() const;
    /**
     * @brief Get number of Codepoints
     * @return Number of CodePoints in UTF8String
     */
    size_t get_point_count() const;
    /**
     * @brief Return n-th CodePoint in UTF8String
     * @param index Index of CodePoint
     * @return CodePoint on index or std::nullopt
     */
    std::optional<CodePoint> nth_code_point(const size_t &index) const;

    BytesIterator bytes() const;

    CodePointIterator codepoints() const;

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
    std::optional<uint8_t> operator[](const size_t &index) const;
    UTF8String operator+(const UTF8String &right) const; ///< + operator to concatenate UTF8Strings
    operator std::string() const;                        ///< Cast UTF8String to std::string

    UTF8String &operator=(const UTF8String &right);  ///< Assing operator
    UTF8String &operator+=(const UTF8String &right); ///< append UTF8String
};

/// Binary tree
// Big data that we cannot afford to copy
struct BigData
{
    explicit BigData(int value) : value(value) {}

    BigData(const BigData &) = delete;
    BigData &operator=(const BigData &) = delete;

    int value;
};

class Tree;
