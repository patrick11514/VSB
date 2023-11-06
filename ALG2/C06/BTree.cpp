#include "BTree.h"
#include <iostream>

BTree::BTree(const int N)
{
    this->root = new BTreePage(N);
}

BTree::~BTree()
{
    delete this->root;
}

void BTree::insert(const int value)
{
    BTreePage *newRoot = this->root->insert(value, true);

    if (newRoot != nullptr)
    {
        this->root = newRoot;
    }
}

void BTree::print() const
{
    if (this->root != nullptr)
    {
        this->root->print();
    }
}

bool BTree::searchValue(const int value) const
{
    if (this->root == nullptr)
        return false;

    return this->root->searchValue(value);
}

int BTree::countKeys() const
{
    if (this->root == nullptr)
        return 0;

    return this->root->countKeys();
}

int BTree::capacity() const
{
    if (this->root == nullptr)
        return 0;

    return this->root->capacity();
}

double BTree::fillFactor() const
{
    if (this->root == nullptr)
        return 0;

    return this->root->countKeys() / (double)this->root->capacity();
}

int BTree::height() const
{
    if (this->root == nullptr)
        return 0;

    return this->root->height();
}
