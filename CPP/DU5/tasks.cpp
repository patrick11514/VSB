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

        // mode first bytes to left
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

UTF8String::UTF8String(const UTF8String &instance)
{
    this->data.reserve(instance.data.size());
    this->data.assign(instance.data.begin(), instance.data.end());
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
    // prý jde něco takovýho, ale idk XD std::string str(this->data, this->size);
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
    // převzato ze staré implementace a nechtělo se mi řešit elegantnější řešení :/
    std::vector<CodePoint> cps = this->_decodeCodePoints();

    if (index >= cps.size())
    {
        return std::nullopt;
    }

    return cps[index];
}
