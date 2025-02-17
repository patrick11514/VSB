#pragma once

#include "BTreePage.h"

/**
 * @brief BTree class
 */
class BTree
{
private:
    /**
     * @brief root - pointer to the root page
     */
    BTreePage *root;

public:
    /**
     * @brief BTree - constructor
     * @param N - half of the maximum number of keys in a page
     */
    BTree(const int N);
    ~BTree();

    /**
     * @brief insert - insert a value to the tree
     * @param value - value to be inserted
     */
    void insert(const int value);
    /**
     * @brief clear - clear the tree
     */
    void clear();

    /**
     * @brief print - print the tree
     */
    void print() const;
    /**
     * @brief searchValue - search a value in the tree
     * @param value - value to be searched
     * @return true if the value is found, false otherwise
     */
    bool searchValue(const int value) const;
    /**
     * @brief countKeys - count the number of keys in the tree
     * @return the number of keys in the tree
     */
    int countKeys() const;
    /**
     * @brief capacity - get the capacity of the tree
     * @return the capacity of the tree
     */
    int capacity() const;
    /**
     * @brief fillFactor - get the fill factor of the tree
     * @return the fill factor of the tree
     */
    double fillFactor() const;
    /**
     * @brief height - get the height of the tree
     * @return the height of the tree
     */
    int height() const;
};