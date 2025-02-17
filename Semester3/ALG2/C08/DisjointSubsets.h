#pragma once

#include <vector>
#include <string>
#include <map>

class DisjointSubsets
{
private:
    const int noParent = -1;
    std::map<std::string, int> elementsMap;
    std::vector<int> parents;

public:
    DisjointSubsets(const std::vector<std::string> &elements);

    int find(const std::string &x) const;

    void Union(const std::string &x, const std::string &y);
};