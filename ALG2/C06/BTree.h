#pragma once

#include "BTreePage.h"

class BTree
{
private:
    BTreePage *root;

public:
    BTree(const int n);
    ~BTree();

    bool search(const int key) const;
    void insert(const int key);
    void clear();
    int countKeys() const;
};