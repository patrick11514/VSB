#include "tasks.h"

int UTF8String::_getClosestPowerOfTwo(int x) const
{
    int base = 2;
    for (int i = 0; i < x; ++i)
    {
        base *= 2;

        if (base > x)
        {
            return base;
        }
    }

    return base;
}

std::vector<uint16_t> UTF8String::_decodeCodePoint(CodePoint cp) const
{
    std::vector<uint16_t> output;

    if (cp <= 0x7F)
    {
        output.push_back(static_cast<uint16_t>(cp));
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

        uint16_t first = cp >> (6 * parts);
        output.push_back(first | this->_createMask(parts));

        for (int i = parts - 1; i >= 0; --i)
        {
            uint16_t shifted = (cp >> i * 6) & 0b111111;
            output.push_back(shifted | 0b10000000);
        }
    }

    return output;
}

std::vector<uint16_t> UTF8String::_decodeCodePoints(std::vector<CodePoint> input) const
{
    std::vector<uint16_t> output;

    for (auto cp : input)
    {
        std::vector<uint16_t> temp = this->_decodeCodePoint(cp);
        output.insert(output.end(), temp.begin(), temp.end());
    }

    return output;
}

std::vector<CodePoint> UTF8String::_encodeCodePoints() const
{
    std::vector<CodePoint> output;

    for (int i = 0; i < this->size; ++i)
    {
        uint16_t byte = this->data[i];

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

void UTF8String::_resizeBuffer(int minSize = 0)
{
    int prev = this->capacity;

    do
    {
        this->capacity = this->capacity > 0 ? this->capacity * 2 : 1;
    } while (this->capacity < minSize + prev);

    uint16_t *oldData = this->data;
    this->data = new uint16_t[this->capacity];
    memcpy(this->data, oldData, this->size * 2);
    delete[] oldData;
}

UTF8String::UTF8String(std::string str)
{
    this->size = str.size();
    this->capacity = this->_getClosestPowerOfTwo(this->size);
    this->data = new uint16_t[this->capacity];

    for (int i = 0; i < this->size; ++i)
    {
        this->data[i] = static_cast<uint16_t>(str[i]);
    }
}

UTF8String::UTF8String(std::vector<CodePoint> cps)
{
    std::vector<uint16_t> bytes = this->_decodeCodePoints(cps);
    this->size = bytes.size();
    this->capacity = this->_getClosestPowerOfTwo(bytes.size());
    this->data = new uint16_t[this->capacity];

    for (size_t i = 0; i < bytes.size(); ++i)
    {
        this->data[i] = bytes[i];
    }
}

UTF8String::UTF8String(const UTF8String &instance)
{
    this->size = instance.size;
    this->capacity = instance.capacity;
    this->data = new uint16_t[this->capacity];
    memcpy(this->data, instance.data, this->size * 2);
}

UTF8String::~UTF8String()
{
    if (this->data != nullptr)
    {
        delete[] this->data;
    }
}

void UTF8String::append(char c)
{
    if (this->size + 1 > this->capacity)
    {
        this->_resizeBuffer();
    }

    this->data[this->size] = static_cast<uint16_t>(c);
    this->size++;
}

void UTF8String::append(CodePoint cp)
{
    std::vector<uint16_t> bytes = this->_decodeCodePoint(cp);

    if (this->size + static_cast<int>(bytes.size()) > this->capacity)
    {
        this->_resizeBuffer(bytes.size());
    }

    for (size_t i = 0; i < bytes.size(); ++i)
    {
        this->data[this->size] = bytes[i];
        this->size++;
    }
}

bool UTF8String::operator==(const UTF8String &right) const
{
    if (this->size != right.size)
    {
        return false;
    }

    for (int i = 0; i < this->size; ++i)
    {
        if (this->data[i] != right.data[i])
        {
            return false;
        }
    }

    return true;
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

    if (this->data != nullptr)
    {
        delete[] this->data;
    }
    this->size = right.size;
    this->capacity = right.capacity;
    this->data = new uint16_t[this->capacity];
    memcpy(this->data, right.data, this->size * 2);

    return *this;
}

std::optional<uint16_t> UTF8String::operator[](const int &index) const
{
    if (index >= this->size || index < 0)
    {
        return std::nullopt;
    }

    return this->data[index];
}

UTF8String UTF8String::operator+(const UTF8String &right) const
{
    UTF8String final;
    final._resizeBuffer(this->size + right.size);
    memcpy(final.data, this->data, this->size * 2);
    memcpy(final.data + this->size, right.data, right.size * 2);
    final.size = this->size + right.size;
    return final;
}

UTF8String::operator std::string() const
{
    std::string str;
    str.reserve(this->size);

    for (int i = 0; i < this->size; ++i)
    {
        str += static_cast<char>(this->data[i]);
    }

    return str;
}

UTF8String &UTF8String::operator+=(const UTF8String &right)
{
    if (this->size + right.size > this->capacity)
    {
        this->_resizeBuffer(this->size + right.size);
    }

    memcpy(this->data + this->size, right.data, right.size * 2);
    this->size += right.size;
    return *this;
}

int UTF8String::get_byte_count() const
{
    return this->size;
}

int UTF8String::get_point_count() const
{
    std::vector<CodePoint> cps = this->_encodeCodePoints();

    return cps.size();
}

std::optional<CodePoint> UTF8String::nth_code_point(const int &index) const
{
    if (index < 0)
    {
        return std::nullopt;
    }

    std::vector<CodePoint> cps = this->_encodeCodePoints();

    if (index >= static_cast<int>(cps.size()))
    {
        return std::nullopt;
    }

    return cps[index];
}
