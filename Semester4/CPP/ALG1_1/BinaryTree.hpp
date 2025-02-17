#pragma once

class BinaryTree
{
    int value;
    BinaryTree *left = nullptr;
    BinaryTree *right = nullptr;

public:
    BinaryTree(int value);
    ~BinaryTree();

    void setRight(BinaryTree *child);
    void setLeft(BinaryTree *child);

    BinaryTree *getRight() const;
    BinaryTree *getLeft() const;
    int getValue() const;
    void print() const;
};