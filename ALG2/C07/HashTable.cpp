#include "HashTable.h"
#include <iostream>

void HashTable::init(const int size)
{
    this->size = size;
    this->slots = std::vector<std::vector<std::pair<std::string, int>>>(this->size);
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
