#ifndef __Graph_H_
#define __Graph_H_ 1

#include <string>
#include <unordered_map>

#include "Node.h"

class Graph
{
private:
    std::unordered_map<int, Node *> nodes;
    std::vector<Node *> biggestComponentNodes;

    int biggestComponent = -1;
    int diameter = -1;
    int radius = -1;

public:
    Graph(std::string fileName);
    ~Graph();

    void resetBiggestComponent();
    int getBiggestComponent();

    void resetDiameter();
    int getDiameter();

    void resetRadius();
    int getRadius();

    void resetGraph();

    std::vector<Node *> getBiggestComponentNodes();
};

#endif
