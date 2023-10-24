#include "BTree.h"

BTree::BTree(const int n)
{
    this->root = new BTreePage(n);
}

BTree::~BTree()
{
    delete this->root;
}

void BTree::insert(const int key)
{
    this->root->insert(key);
}

int BTree::countKeys() const
{
    return this->root->count();
}
