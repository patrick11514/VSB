#pragma once

#include <memory>
#include <cstdint>
#include <string>
#include <functional>
#include <iostream>
#include <utility>
#include <cstring>
#include <optional>
#include <exception>
#include <queue>
#include <stack>

/**
   ____      _                          _ _                  ____  _            _
  / __ \    | |                        | (_)                / __ \| |          | |
 | |  | | __| |_ __   _____   _____  __| |_   _ __   __ _  | |  | | |_ __ _ ___| | ___   _
 | |  | |/ _` | '_ \ / _ \ \ / / _ \/ _` | | | '_ \ / _` | | |  | | __/ _` |_  / |/ / | | |
 | |__| | (_| | |_) | (_) \ V /  __/ (_| | | | | | | (_| | | |__| | || (_| |/ /|   <| |_| |
  \____/ \__,_| .__/ \___/ \_/ \___|\__,_|_| |_| |_|\__,_|  \____/ \__\__,_/___|_|\_\\__, |
              | |                                                                     __/ |
              |_|                                                                    |___/

[*] What type of pointer should be used for the value and what type should be used for storing
the child nodes? What type of pointer should be used for the parent link? Think about ownership -
who is the owner of what data and which data is shared by whom.

Pro hodnotu bude použit shared pointer, protože hodnotu, tím že může být velká, tak chci sdílet mezi
třeba více nodama, za to potomky budou unique pointer, protože potomek je vlastněn daným parentem.

*/

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
    size_t _getCountOfLeadingParts() const;

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

    /**
     * @brief Return iterator over bytes in UTF8String
     * @return Iterator over bytes
     */
    BytesIterator bytes() const;

    /**
     * @brief Return iterator over CodePoints in UTF8String
     * @return Iterator over CodePoints
     */
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

class Tree
{
    std::shared_ptr<BigData> data;
    Tree *parent;
    std::unique_ptr<Tree> left;
    std::unique_ptr<Tree> right;

public:
    Tree(int data);
    Tree(std::shared_ptr<BigData> data);

    Tree *get_parent() const;
    bool has_parent() const;
    Tree *get_left_child() const;
    Tree *get_right_child() const;
    BigData &get_value() const;
    const Tree *get_root() const;
    bool is_same_tree_as(Tree *other) const;

    std::unique_ptr<Tree> set_left_child(std::unique_ptr<Tree> child);
    std::unique_ptr<Tree> set_right_child(std::unique_ptr<Tree> child);
    std::unique_ptr<Tree> take_left_child();
    std::unique_ptr<Tree> take_right_child();
    std::unique_ptr<Tree> take_child(Tree &child);
    void swap_children();
    void replace_value(std::shared_ptr<BigData> value);

    class TreeIterator
    {
        std::vector<Tree *> data;
        size_t current;

        void _putIntoData(Tree *node);

    public:
        TreeIterator(Tree *root, bool end);

        bool operator==(const TreeIterator &other) const;
        bool operator!=(const TreeIterator &other) const;
        TreeIterator &operator++();
        TreeIterator operator++(int);
        TreeIterator &operator--();
        TreeIterator operator--(int);
        Tree &operator*();
    };

    Tree::TreeIterator begin();
    Tree::TreeIterator end();
};
