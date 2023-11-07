#pragma once
#include <string>
#include <utility>
#include <vector>

class HashTable
{
private:
    int size;
    std::vector<std::vector<std::pair<std::string, int>>> slots;

    int hash(const std::string &key) const;

public:
    /**
     * @brief Create hash table of size 100
     */
    HashTable();
    /**
     * @brief Create hash table of size size
     * @param size Size of hash table
     */
    HashTable(const int size);
    /**
     * @brief Destroy hash table
     */
    ~HashTable();

    /**
     * @brief Insert key and value into hash table, if key exists, update value
     * @param key Key to insert
     * @param value Value to insert
     */
    void insert(const std::string &key, const int value);
    /**
     * @brief Remove key from hash table
     */
    void remove(const std::string &key);
    /**
     * @brief Clear hash table
     */
    void clear();

    /**
     * @brief Check if key exists in hash table
     * @param key Key to check
     * @return True if key exists, false otherwise
     */
    bool contains(const std::string &key) const;
    /**
     * @brief Try to get value of key
     * @param key Key to get value of
     * @param value Value of key
     * @return True if key exists, false otherwise
     */
    bool tryGetValue(const std::string &key, int &value) const;
};