#pragma once
#include <vector>

class BTreePage
{
private:
    std::vector<int> data;
    std::vector<BTreePage *> children;
    int n;

    void _insert(const int value);

public:
    BTreePage(int n);
    ~BTreePage();

    int count() const;
    void insert(const int value);

    void print() const;
};