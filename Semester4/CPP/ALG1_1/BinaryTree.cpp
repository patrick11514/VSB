#include "BinaryTree.hpp"
#include <iostream>

BinaryTree::BinaryTree(int value) : value(value) {}

BinaryTree::~BinaryTree()
{
    delete this->left;
    delete this->right;
}

void BinaryTree::setRight(BinaryTree *child)
{
    this->right = child;
}

void BinaryTree::setLeft(BinaryTree *child)
{
    this->left = child;
}

BinaryTree *BinaryTree::getRight() const
{
    return this->right;
}

BinaryTree *BinaryTree::getLeft() const
{
    return this->left;
}

int BinaryTree::getValue() const
{
    return this->value;
}

void BinaryTree::print() const
{
    std::cout << this->value << " | ";

    if (this->left != nullptr)
    {
        this->left->print();
        std::cout << " | ";
    }

    if (this->right != nullptr)
    {
        this->right->print();
    }
    std::endl(std::cout);
}
