#pragma once
#include <string>
#include <utility>
#include <vector>

/**
 * @brief Hash table
 */
class HashTable
{
private:
    /**
     * @brief Slots size of hash table
     */
    int size;
    /**
     * @brief Slots of hash table
     */
    std::vector<std::vector<std::pair<std::string, int>>> slots;
    /**
     * @brief Initialize hash table
     * @param size Size of hash table
     */
    void init(const int size);
    /**
     * @brief Jenkins hash function
     * @link https://en.wikipedia.org/wiki/Jenkins_hash_function
     */
    unsigned int hash(const std::string &key) const;

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
     * @brief Get/Set value of key
     * @param key Key to Get/Set value of key
     * @return Value of key, 0 if key does not exist
     */
    int &operator[](const std::string &key);
    /**
     * @brief Get size of hash table
     * @return Size of hash table
     */
    size_t getTableSize() const;
    /**
     * @brief Get number of keys in hash table
     * @return Number of keys in hash table
     */
    size_t getNumberOfKeys() const;
    /**
     * @brief Get load factor of hash table
     * @return Load factor of hash table
     */
    double getLoadFactor() const;

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
    bool tryGetValue(const std::string &key, int *value) const;
    /**
     * @brief Get value of key
     * @param key Key to get value of
     * @return Value of key, 0 if key does not exist
     */
    int operator[](const std::string &key) const;
};