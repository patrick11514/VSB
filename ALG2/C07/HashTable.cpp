#include "HashTable.h"
#include <iostream>

void HashTable::init(const int size)
{
    this->size = size;
    this->slots = std::vector<std::vector<std::pair<std::string, int>>>(this->size);
}

unsigned int HashTable::hash(const std::string &key) const
{
    int i = 0;
    unsigned int hash = 0;
    while (i != key.length())
    {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

HashTable::HashTable()
{
    this->init(100);
}

HashTable::HashTable(const int size)
{
    this->init(size);
}

HashTable::~HashTable()
{
}

void HashTable::insert(const std::string &key, const int value)
{
    unsigned int hash = this->hash(key) % this->size;

    for (auto &slotItem : this->slots[hash])
    {
        if (slotItem.first.compare(key) == 0)
        {
            slotItem.second = value;
            return;
        }
    }

    this->slots[hash].push_back(std::pair<std::string, int>(key, value));
}

void HashTable::remove(const std::string &key)
{
    int32_t hash = this->hash(key) % this->size;
    for (auto i = this->slots[hash].begin(); i != this->slots[hash].end(); i++)
    {
        if (i.base()->first == key)
        {
            this->slots[hash].erase(i);
            return;
        }
    }
}

void HashTable::clear()
{
    for (int i = 0; i < this->size; ++i)
    {
        this->slots[i].clear();
    }
}

bool HashTable::contains(const std::string &key) const
{
    int32_t hash = this->hash(key) % this->size;

    for (auto slotItem : this->slots[hash])
    {
        if (slotItem.first == key)
        {
            return true;
        }
    }

    return false;
}

bool HashTable::tryGetValue(const std::string &key, int *value) const
{
    int32_t hash = this->hash(key) % this->size;

    for (auto slotItem : this->slots[hash])
    {
        if (slotItem.first == key)
        {
            *value = slotItem.second;
            return true;
        }
    }

    return false;
}

int HashTable::operator[](const std::string &key) const
{
    int32_t hash = this->hash(key) % this->size;

    for (auto slotItem : this->slots[hash])
    {
        if (slotItem.first == key)
        {
            return slotItem.second;
        }
    }

    return 0;
}

int &HashTable::operator[](const std::string &key)
{
    int32_t hash = this->hash(key) % this->size;

    for (auto &slotItem : this->slots[hash])
    {

        if (slotItem.first == key)
        {
            return slotItem.second;
        }
    }

    this->insert(key, 0);

    return this->operator[](key);
}

size_t HashTable::getTableSize() const
{
    return this->size;
}

size_t HashTable::getNumberOfKeys() const
{
    size_t size = 0;

    for (auto slot : this->slots)
    {
        size += slot.size();
    }
    return size;
}

double HashTable::getLoadFactor() const
{
    int fulledSlots = 0;

    for (auto slot : this->slots)
    {
        if (slot.size() > 0)
        {
            fulledSlots++;
        }
    }
    return (double)fulledSlots / (double)this->size;
}
