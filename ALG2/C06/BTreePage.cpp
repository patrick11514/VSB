#include "BTreePage.h"
#include <iostream>

void BTreePage::init(const int N)
{
    this->N = N;
    // + 2 beacause of overflow in splitting process
    std::vector<BTreePage *> children(N * 2 + 2, nullptr);
    this->children = children;
    this->father = nullptr;
}

BTreePage *BTreePage::_insert(const int value)
{
    bool inserted = false;
    BTreePage *newRoot = this->_insertToChild(value, &inserted);
    if (inserted)
    {
        return newRoot;
    }

    this->normalInsert(value);

    return nullptr;
}

BTreePage *BTreePage::_insertToChild(const int value, bool *inserted)
{
    for (int i = this->N * 2; i > 0; --i)
    {
        if (this->children[i] != nullptr)
        {
            int myValue = this->values[i - 1];

            // insert to right side
            if (value > myValue)
            {
                BTreePage *newRoot = this->children[i]->insert(value, true);
                *inserted = true;
                return newRoot;
            }
            // insert to left side
            else
            {
                BTreePage *newRoot = this->children[i - 1]->insert(value, true);
                *inserted = true;
                return newRoot;
            }
        }
    }

    return nullptr;
}

BTreePage::BTreePage(const int N)
{
    this->init(N);
}

BTreePage::BTreePage(const int N, const int value, BTreePage *LChild, BTreePage *RChild)
{
    this->init(N);
    this->values.push_back(value);
    this->children[0] = LChild;
    this->children[1] = RChild;
}

BTreePage::~BTreePage()
{
    for (auto page : this->children)
    {
        delete page;
    }
}

BTreePage *BTreePage::insert(const int value, const bool toChildren)
{
    // insert data to children
    if (toChildren)
    {
        BTreePage *newRoot = this->_insert(value);
        if (newRoot != nullptr)
        {
            return newRoot;
        }
    }
    // because if we want to insert data to father in case of splitting
    else
    {
        this->normalInsert(value);
    }

    // if page is overflowed (illegal state), resolve it by splitting the page
    if (this->isOverflow())
    {
        return this->resolveOverflow();
    }

    return nullptr;
}

void BTreePage::normalInsert(const int value)
{
    this->values.push_back(value);

    for (std::vector<int>::size_type i = this->values.size() - 1; i > 0; --i)
    {
        if (this->values[i - 1] > this->values[i])
        {
            // swap values
            int tmp = this->values[i - 1];
            this->values[i - 1] = this->values[i];
            this->values[i] = tmp;

            // swap pointers
            BTreePage *tmpPage = this->children[i];
            this->children[i] = this->children[i + 1];
            this->children[i + 1] = tmpPage;
        }
    }
}

BTreePage *BTreePage::resolveOverflow()
{
    // create right page
    BTreePage *rightPage = new BTreePage(this->N);
    // insert the first child
    rightPage->insertFirstChild(this->children[this->N + 1]);
    for (int i = this->N * 2; i > this->N; --i)
    {
        // insert other values
        rightPage->insert(this->values[i], false);
        rightPage->insertChild(this->values[i], this->children[i + 1]);
    }

    BTreePage *rootPage = nullptr;

    // if father page doesn't exist, create it
    if (this->father == nullptr)
    {
        rootPage = new BTreePage(this->N, this->values[N], this, rightPage);

        this->father = rootPage;
        rightPage->father = rootPage;
    }
    else
    {
        // set father page to right page
        rightPage->father = this->father;

        // insert the middle value to the father page
        this->father->normalInsert(this->values[this->N]);
        this->father->insertChild(this->values[this->N], rightPage);

        // if father page is overflowed, resolve it
        if (this->father->isOverflow())
        {
            rootPage = this->father->resolveOverflow();
        }
    }

    // delete the values moved to right page and father page
    for (int i = 0; i < this->N + 1; ++i)
    {
        this->values.pop_back();
    }

    // delete the children moved to right page and father page
    for (int i = this->N * 2 + 1; i > this->N; --i)
    {
        this->children[i] = nullptr;
    }

    // return new root page to be updated in BTree
    return rootPage;
}

void BTreePage::insertChild(const int value, BTreePage *page)
{
    for (std::vector<int>::size_type i = 0; i < this->values.size(); ++i)
    {
        if (value == this->values[i])
        {
            this->children[i + 1] = page;
            return;
        }
    }
}

void BTreePage::insertFirstChild(BTreePage *page)
{
    if (this->children[0] == nullptr)
    {
        this->children[0] = page;
    }
}

void BTreePage::clear()
{
    int i = 0;
    for (std::vector<BTreePage *>::size_type i = 0; i < this->children.size(); ++i)
    {
        if (this->children[i] != nullptr)
        {
            delete this->children[i];
            this->children[i] = nullptr;
        }
    }

    int size = this->values.size();

    for (std::vector<int>::size_type i = 0; i < size; ++i)
    {
        this->values.pop_back();
    }
}

void BTreePage::print() const
{
    // print values in the page
    for (int i = 0; i < this->values.size(); ++i)
    {
        std::cout << " " << this->values[i] << " ";
    }

    std::cout << std::endl;

    // add start + number for each child
    for (int i = 0; i < this->children.size(); ++i)
    {
        if (this->children[i] != nullptr)
        {
            std::cout << "*" << i << " ";
        }
    }

    std::cout << std::endl;

    // print children with numbered star
    for (int i = 0; i < this->children.size(); ++i)
    {
        if (this->children[i] != nullptr)
        {
            std::cout << std::endl
                      << "*" << i << ":" << std::endl;

            this->children[i]->print();
        }
    }
}

bool BTreePage::isOverflow() const
{
    return this->values.size() > this->N * 2;
}

bool BTreePage::searchValue(const int value) const
{
    for (int i : this->values)
    {
        if (i == value)
        {
            return true;
        }
    }

    for (BTreePage *child : this->children)
    {
        if (child != nullptr)
        {
            bool found = child->searchValue(value);
            if (found)
            {
                return true;
            }
        }
    }

    return false;
}

int BTreePage::countKeys() const
{
    int count = this->values.size();

    for (BTreePage *child : this->children)
    {
        if (child != nullptr)
        {
            count += child->countKeys();
        }
    }

    return count;
}

int BTreePage::capacity() const
{
    int capacity = this->N * 2;

    for (BTreePage *child : this->children)
    {
        if (child != nullptr)
        {
            capacity += child->capacity();
        }
    }

    return capacity;
}

int BTreePage::height() const
{
    int height = 1;

    for (BTreePage *child : this->children)
    {
        if (child != nullptr)
        {
            int childHeight = child->height() + 1;
            if (childHeight > height)
            {
                height = childHeight;
            }
        }
    }

    return height;
}
