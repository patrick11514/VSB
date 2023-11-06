#pragma once

#include <vector>

/**
 * @brief BTreePage class
 */
class BTreePage
{
private:
    /**
     * @brief N - half of the maximum number of keys in a page
     */
    int N;

    /**
     * @brief values - vector of keys
     */
    std::vector<int> values;
    /**
     * @brief children - vector of children
     */
    std::vector<BTreePage *> children;

    /**
     * @brief init - initialize the page
     * @param N - half of the maximum number of keys in a page
     */
    void init(const int N);
    /**
     * @brief insert - insert a value to the page
     * @param value - value to be inserted
     */
    BTreePage *_insert(const int value);
    /**
     * @brief insertToChild - insert a value to a child
     * @param value - value to be inserted
     * @param inserted - pointer to a boolean that indicates if the value was inserted
     */
    BTreePage *_insertToChild(const int value, bool *inserted);

public:
    /**
     * @brief father - pointer to the father page
     */
    BTreePage *father;

    /**
     * @brief BTreePage - constructor
     * @param N - half of the maximum number of keys in a page
     */
    BTreePage(const int N);
    /**
     * @brief BTreePage - constructor, which is used in splitting process to create father page
     * @param N - half of the maximum number of keys in a page
     * @param value - value to be inserted
     * @param LChild - pointer to the left child
     * @param RChild - pointer to the right child
     */
    BTreePage(const int N, const int value, BTreePage *LChild, BTreePage *RChild);
    /**
     * @brief ~BTreePage - destructor
     */
    ~BTreePage();

    /**
     * @brief insert - insert a value to the page
     * @param value - value to be inserted
     * @param toChildren - indicates if the value should be inserted to a child (in case of splitting, we don't want to insert to a child)
     * @return pointer to the new root of the tree
     */
    BTreePage *insert(const int value, const bool toChildren);
    /**
     * @brief normalInsert - insert a value to the page
     */
    void normalInsert(const int value);
    /**
     * @brief resolveOverflow - resolve an overflowed page by splitting it
     * @return pointer to the new root of the tree
     */
    BTreePage *resolveOverflow();
    /**
     * insertChild - insert a child to the page
     * @param value - value for what the child will be inserted
     * @param page - pointer to the child
     */
    void insertChild(const int value, BTreePage *page);
    /**
     * @brief insertFirstChild - insert a child to the first position of the page
     * @param page - pointer to the child
     */
    void insertFirstChild(BTreePage *page);

    /**
     * @brief print - print the page and its children
     */
    void print() const;
    /**
     * @brief isOverflow - check if the page is overflowed
     * @return true if the page is overflowed, false otherwise
     */
    bool isOverflow() const;
    /**
     * @brief searchValue - search a value in the page
     * @param value - value to be searched
     * @return true if the value is found, false otherwise
     */
    bool searchValue(const int value) const;
    /**
     * @brief countKeys - count the number of keys in the page
     * @return the number of keys in the page
     */
    int countKeys() const;
    /**
     * @brief capacity - get the capacity of the tree
     * @return the capacity of the tree
     */
    int capacity() const;
    /**
     * @brief height - get the height of the tree
     * @return the height of the tree
     */
    int height() const;
};