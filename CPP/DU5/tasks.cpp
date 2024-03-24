#include "tasks.h"

std::vector<uint8_t> UTF8String::_encodeCodePoint(CodePoint cp) const
{
    std::vector<uint8_t> output;

    if (cp <= 0x7F)
    {
        output.push_back(static_cast<uint8_t>(cp));
    }
    else
    {

        int parts = 0;

        if (cp <= 0x7FF)
        {
            parts = 1;
        }
        else if (cp <= 0xFFFF)
        {
            parts = 2;
        }
        else
        {
            parts = 3;
        }

        uint8_t first = cp >> (6 * parts);
        output.push_back(first | this->_createMask(parts));

        for (int i = parts - 1; i >= 0; --i)
        {
            uint8_t shifted = (cp >> i * 6) & 0b111111;
            output.push_back(shifted | 0b10000000);
        }
    }

    return output;
}

std::vector<uint8_t> UTF8String::_encodeCodePoints(std::vector<CodePoint> cps) const
{
    std::vector<uint8_t> output;

    for (auto cp : cps)
    {
        std::vector<uint8_t> temp = this->_encodeCodePoint(cp);
        output.insert(output.end(), temp.begin(), temp.end());
    }

    return output;
}

std::vector<CodePoint> UTF8String::_decodeCodePoints() const
{
    std::vector<CodePoint> output;

    for (size_t i = 0; i < this->data.size(); ++i)
    {
        uint8_t byte = this->data[i];

        int parts = 0;
        // U+0000 - U+007F
        if (byte >> 7 == 0b0)
        {
            // U+0000 - U+007F
            output.push_back(static_cast<CodePoint>(byte));
            continue;
        }
        else if (byte >> 5 == 0b110)
        {
            // separate only bits, which we care
            byte &= 0b11111;
            parts = 1;
        }
        else if (byte >> 4 == 0b01110)
        {
            byte &= 0b1111;
            parts = 2;
        }
        else
        {
            byte &= 0b111;
            parts = 3;
        }

        // move first bytes to left
        CodePoint final = byte << (6 * parts);

        for (int l = parts - 1; l >= 0; --l)
        {
            int offset = parts - l;
            final |= (this->data[i + offset] & 0b111111) << l * 6;
        }

        i += parts;

        output.push_back(final);
    }

    return output;
}

int UTF8String::_createMask(int ones) const
{
    int base = 1;
    int oringNumber = 1;

    for (int i = 0; i < 7; ++i)
    {
        base <<= 1;
        base |= oringNumber;
        if (ones > 0)
        {
            ones -= 1;
            if (ones == 0)
            {
                oringNumber = 0;
            }
        }
    }

    return base;
}

UTF8String::UTF8String(const std::string &str) : data(str.begin(), str.end())
{
}

UTF8String::UTF8String(const std::vector<CodePoint> &cps)
{
    std::vector<uint8_t> bytes = this->_encodeCodePoints(cps);

    this->data.assign(bytes.begin(), bytes.end());
}

UTF8String::UTF8String(const std::vector<uint8_t> &bytes) : data(bytes)
{
}

UTF8String::UTF8String(const UTF8String &instance)
{
    this->data.reserve(instance.data.size());
    this->data.assign(instance.data.begin(), instance.data.end());
}

UTF8String::UTF8String(UTF8String &&instance)
{
    this->data.swap(instance.data);
}

void UTF8String::append(char c)
{
    this->data.push_back(static_cast<uint8_t>(c));
}

void UTF8String::append(CodePoint cp)
{
    std::vector<uint8_t> bytes = this->_encodeCodePoint(cp);

    this->data.reserve(this->data.size() + bytes.size());

    for (auto byte : bytes)
    {
        this->data.emplace_back(byte);
    }
}

bool UTF8String::operator==(const UTF8String &right) const
{
    return std::equal(this->data.begin(), this->data.end(), right.data.begin(), right.data.end());
}

bool UTF8String::operator!=(const UTF8String &right) const
{
    return !(this->operator==(right));
}

UTF8String &UTF8String::operator=(const UTF8String &right)
{
    // self assign handling
    if (this->data == right.data)
    {
        return *this;
    }

    this->data.assign(right.data.begin(), right.data.end());

    return *this;
}

std::optional<uint8_t> UTF8String::operator[](const size_t &index) const
{
    if (index >= this->data.size())
    {
        return std::nullopt;
    }

    return this->data[index];
}

UTF8String UTF8String::operator+(const UTF8String &right) const
{
    UTF8String final;
    final.data.reserve(this->data.size() + right.data.size());
    final.data.insert(final.data.end(), this->data.begin(), this->data.end());
    final.data.insert(final.data.end(), right.data.begin(), right.data.end());
    return final;
}

UTF8String::operator std::string() const
{
    std::string str(this->data.begin(), this->data.end());

    return str;
}

UTF8String &UTF8String::operator+=(const UTF8String &right)
{
    this->data.reserve(this->data.size() + right.data.size());
    this->data.insert(this->data.end(), right.data.begin(), right.data.end());

    return *this;
}

size_t UTF8String::get_byte_count() const
{
    return this->data.size();
}

size_t UTF8String::get_point_count() const
{
    std::vector<CodePoint> cps = this->_decodeCodePoints();

    return cps.size();
}

std::optional<CodePoint> UTF8String::nth_code_point(const size_t &index) const
{
    // převzato ze staré implementace a nechtělo se mi řešit elegantnější řešení
    std::vector<CodePoint> cps = this->_decodeCodePoints();

    if (index >= cps.size())
    {
        return std::nullopt;
    }

    return cps[index];
}

BytesIterator UTF8String::bytes() const
{
    return BytesIterator(this->data.data(), this->data.size());
}

CodePointIterator UTF8String::codepoints() const
{
    return CodePointIterator(this->data.data(), this->data.size());
}

BytesIterator::BytesIterator(const uint8_t *bytes, size_t size) : data(bytes), size(size)
{
}

BytesIterator BytesIterator::begin() const
{
    return BytesIterator(this->data, this->size);
}

BytesIterator BytesIterator::end() const
{
    return BytesIterator(this->data + this->size, this->size);
}

uint8_t BytesIterator::operator*() const
{
    return *this->data;
}

bool BytesIterator::operator==(const BytesIterator &other) const
{
    return this->data == other.data;
}

bool BytesIterator::operator!=(const BytesIterator &other) const
{
    return !this->operator==(other);
}

BytesIterator &BytesIterator::operator++()
{
    this->data++;
    return *this;
}

BytesIterator BytesIterator::operator++(int)
{
    BytesIterator pre = *this;
    this->data++;
    return pre;
}

BytesIterator &BytesIterator::operator--()
{
    this->data--;
    return *this;
}

BytesIterator BytesIterator::operator--(int)
{
    BytesIterator pre = *this;
    this->data--;
    return pre;
}

BytesIterator BytesIterator::operator+=(size_t value)
{
    this->data += value;
    return *this;
}

BytesIterator BytesIterator::operator-=(size_t value)
{
    this->data -= value;
    return *this;
}

BytesIterator BytesIterator::operator+(size_t value)
{
    this->data = this->data + value;
    return *this;
}

BytesIterator BytesIterator::operator-(size_t value)
{
    this->data = this->data - value;
    return *this;
}

CodePoint CodePointIterator::_getCodePoint() const
{
    uint8_t byte = *this->data;
    int parts = 0;
    // U+0000 - U+007F
    if (byte >> 7 == 0b0)
    {
        // U+0000 - U+007F
        return static_cast<CodePoint>(byte);
    }
    else if (byte >> 5 == 0b110)
    {
        // separate only bits, which we care
        byte &= 0b11111;
        parts = 1;
    }
    else if (byte >> 4 == 0b01110)
    {
        byte &= 0b1111;
        parts = 2;
    }
    else
    {
        byte &= 0b111;
        parts = 3;
    }

    // move first bytes to left
    CodePoint final = byte << (6 * parts);

    for (int l = parts - 1; l >= 0; --l)
    {
        int offset = parts - l;
        final |= (this->data[offset] & 0b111111) << l * 6;
    }

    return final;
}

size_t CodePointIterator::_getCountOfLeadingParts() const
{
    uint8_t byte = *this->data;
    int parts = 0;
    // U+0000 - U+007F
    if (byte >> 7 == 0b0)
    {
        // U+0000 - U+007F
        return 0;
    }
    else if (byte >> 5 == 0b110)
    {
        // separate only bits, which we care
        byte &= 0b11111;
        parts = 1;
    }
    else if (byte >> 4 == 0b01110)
    {
        byte &= 0b1111;
        parts = 2;
    }
    else
    {
        byte &= 0b111;
        parts = 3;
    }
    return parts;
}

CodePointIterator::CodePointIterator(const uint8_t *bytes, size_t size) : data(bytes), size(size)
{
}

CodePointIterator CodePointIterator::begin() const
{
    return CodePointIterator(this->data, this->size);
}

CodePointIterator CodePointIterator::end() const
{
    return CodePointIterator(this->data + this->size, this->size);
}

CodePoint CodePointIterator::operator*() const
{
    return this->_getCodePoint();
}

bool CodePointIterator::operator==(const CodePointIterator &other) const
{
    return this->data == other.data;
}

bool CodePointIterator::operator!=(const CodePointIterator &other) const
{
    return !this->operator==(other);
}

CodePointIterator &CodePointIterator::operator++()
{
    size_t skipBytes = this->_getCountOfLeadingParts();
    // skip bytes + move by one
    this->data += skipBytes + 1;
    return *this;
}

CodePointIterator CodePointIterator::operator++(int)
{
    CodePointIterator pre = *this;
    this->operator++();
    return pre;
}

CodePointIterator &CodePointIterator::operator--()
{
    while (true)
    {
        this->data--;
        // check if we found valid code point starting byte
        if (*this->data >> 6 != 0b10)
        {
            break;
        }
    }

    return *this;
}

CodePointIterator CodePointIterator::operator--(int)
{
    CodePointIterator pre = *this;
    this->operator--();
    return pre;
}

Tree::Tree(int data) : parent(nullptr)
{
    this->data = std::make_unique<BigData>(data);
}

Tree::Tree(std::shared_ptr<BigData> data) : parent(nullptr)
{
    this->data = std::move(data);
}

Tree *Tree::get_parent() const
{
    return this->parent;
}

bool Tree::has_parent() const
{
    return this->parent != nullptr;
}

Tree *Tree::get_left_child() const
{
    return this->left.get();
}

Tree *Tree::get_right_child() const
{

    return this->right.get();
}

BigData &Tree::get_value() const
{
    return *this->data.get();
}

std::unique_ptr<Tree> Tree::set_left_child(std::unique_ptr<Tree> child)
{

    std::unique_ptr<Tree> prev = nullptr;
    if (this->left != nullptr)
    {
        prev = std::move(this->left);
        prev->parent = nullptr;
    }

    this->left = std::move(child);
    if (this->left != nullptr)
    {
        this->left->parent = this;
    }
    return prev;
}

const Tree *Tree::get_root() const
{
    const Tree *current = this;

    while (current->parent != nullptr)
    {
        current = current->parent;
    }
    return current;
}

bool Tree::is_same_tree_as(Tree *other) const
{
    return this->get_root() == other->get_root();
}

std::unique_ptr<Tree> Tree::set_right_child(std::unique_ptr<Tree> child)
{
    std::unique_ptr<Tree> prev = nullptr;
    if (this->right != nullptr)
    {
        prev = std::move(this->right);
        prev->parent = nullptr;
    }
    this->right = std::move(child);
    if (this->right != nullptr)
    {
        this->right->parent = this;
    }
    return prev;
}

std::unique_ptr<Tree> Tree::take_left_child()
{
    std::unique_ptr<Tree> prev = nullptr;
    if (this->left != nullptr)
    {
        prev = std::move(this->left);
        prev->parent = nullptr;
    }

    this->left.reset();

    return prev;
}

std::unique_ptr<Tree> Tree::take_right_child()
{
    std::unique_ptr<Tree> prev = nullptr;
    if (this->right != nullptr)
    {
        prev = std::move(this->right);
        prev->parent = nullptr;
    }

    this->right.reset();

    return prev;
}

std::unique_ptr<Tree> Tree::take_child(Tree &child)
{
    if (this->left.get() == &child)
    {
        return this->take_left_child();
    }
    if (this->right.get() == &child)
    {
        return this->take_right_child();
    }

    throw std::runtime_error("Invalid child");
}

void Tree::swap_children()
{
    std::swap(this->left, this->right);
}

void Tree::replace_value(std::shared_ptr<BigData> value)
{
    std::stack<Tree *> childs;
    childs.push(this);

    while (!childs.empty())
    {
        Tree *node = childs.top();
        node->data = value;
        childs.pop();

        Tree *left = node->get_left_child();

        if (left != nullptr)
        {
            childs.push(left);
        }
        Tree *right = node->get_right_child();

        if (right != nullptr)
        {
            childs.push(right);
        }
    }
}

void Tree::TreeIterator::_putIntoData(Tree *node)
{

    if (node == nullptr)
    {
        return;
    }

    this->_putIntoData(node->get_left_child());

    this->data.push_back(node);

    this->_putIntoData(node->get_right_child());
}

Tree::TreeIterator::TreeIterator(Tree *root, bool end) : current(0)
{
    this->_putIntoData(root);
    if (end)
    {
        this->current = this->data.size();
    }
}

bool Tree::TreeIterator::operator==(const TreeIterator &other) const
{
    return std::equal(this->data.begin(), this->data.end(), other.data.begin(), other.data.end()) && this->current == other.current;
}

bool Tree::TreeIterator::operator!=(const TreeIterator &other) const
{
    return !this->operator==(other);
}

Tree::TreeIterator &Tree::TreeIterator::operator++()
{
    this->current++;
    return *this;
}

Tree::TreeIterator Tree::TreeIterator::operator++(int)
{
    TreeIterator prev = *this;
    this->current++;
    return prev;
}

Tree::TreeIterator &Tree::TreeIterator::operator--()
{
    this->current--;
    return *this;
}

Tree::TreeIterator Tree::TreeIterator::operator--(int)
{
    TreeIterator prev = *this;
    this->current--;
    return prev;
}

Tree &Tree::TreeIterator::operator*()
{
    return *this->data[this->current];
}

Tree::TreeIterator Tree::begin()
{
    return TreeIterator(this, false);
}

Tree::TreeIterator Tree::end()
{
    return TreeIterator(this, true);
}
