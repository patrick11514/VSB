#include "BTreePage.h"
#include <iostream>

void BTreePage::_insert(const int value)
{
    this->data.push_back(value);

    for (std::vector<int>::size_type i = this->data.size() - 1; i > 0; --i)
    {
        if (this->data[i - 1] > this->data[i])
        {
            int tmp = this->data[i];
            this->data[i] = this->data[i - 1];
            this->data[i - 1] = tmp;
            ///
            BTreePage *tmp2 = this->children[i];
            this->children[i] = this->children[i - 1];
            this->children[i - 1] = tmp2;
        }
    }
}

BTreePage::BTreePage(int n)
{
    this->n = n;
    std::vector<BTreePage *> children(n * 2 + 1, nullptr);
    this->children = children;
}

BTreePage::~BTreePage()
{
    for (auto children : this->children)
    {
        if (children != nullptr)
        {
            delete children;
        }
    }
}

int BTreePage::count() const
{
    int count = this->data.size();

    for (auto children : this->children)
    {
        if (children != nullptr)
        {
            count += children->count();
        }
    }

    return count;
}

void BTreePage::insert(const int value)
{
    this->_insert(value);
    if (this->data.size() >= (n * 2))
    {
        BTreePage *L = new BTreePage(this->n);

        for (int i = 0; i < this->n; ++i)
        {
            L->insert(this->data[i]);
        }
        BTreePage *R = new BTreePage(this->n);
        for (int i = n * 2; i >= 0; --i)
        {
            R->insert(this->data[i]);
        }
    }
}

void BTreePage::print() const
{
    for (auto data : this->data)
    {
        std::cout << data << " ";
    }
    std::cout << std::endl;

    for (auto children : this->children)
    {
        if (children != nullptr)
        {
            children->print();
        }
    }
}
