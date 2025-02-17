#include "DisjointSubsets.h"

DisjointSubsets::DisjointSubsets(const std::vector<std::string> &elements)
{
    int i = 0;
    for (const auto &e : elements)
    {
        this->elementsMap.emplace(e, i);
        ++i;
    }

    this->parents.resize(elements.size(), this->noParent);
}

int DisjointSubsets::find(const std::string &x) const
{
    int r = this->elementsMap.at(x);

    while (this->parents[r] != this->noParent)
    {
        r = this->parents[r];
    }

    return r;
}

void DisjointSubsets::Union(const std::string &x, const std::string &y)
{
    int xRoot = this->find(x);
    int yRoot = this->find(y);

    this->parents[yRoot] = xRoot;
}
