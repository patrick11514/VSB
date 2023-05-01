#ifndef __Graph_H_
#define __Graph_H_ 1

#include <string>
#include <unordered_map>

#include "Node.h"

class Graph
{
private:
    std::unordered_map<int, Node *> nodes;

public:
    Graph(std::string fileName);
    ~Graph();
};

#endif
