#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <chrono>

#include "Graph.h"
#include "Node.h"
#include "Types.h"

Graph::Graph(std::string fileName)
{
    std::ifstream file;
    file.open(fileName);

    std::unordered_map<int, Node *> map;

    if (!file.is_open())
    {
        std::cout << "Could not open file" << std::endl;
        return;
    }

    while (!file.eof())
    {
        int from, to;

        file >> from >> to;

        if (file.fail())
        {
            std::cout << "Error reading file" << std::endl;
            return;
        }

        if (map.find(from) == map.end())
        {
            map[from] = new Node(from);
        }

        if (map.find(to) == map.end())
        {
            map[to] = new Node(to);
        }

        map[from]->addNeighbor(map[to]);
        map[to]->addNeighbor(map[from]);
    }

    file.close();

    this->nodes = map;
    this->initialized = true;
}

Graph::~Graph()
{
    for (std::unordered_map<int, Node *>::iterator it = this->nodes.begin(); it != this->nodes.end(); it++)
    {
        delete it->second;
    }
}

void Graph::resetBiggestComponent()
{
    this->biggestComponent = -1;
}

int Graph::getBiggestComponent()
{
    if (this->biggestComponent != -1)
    {
        return this->biggestComponent;
    }

    int biggestComponent = 0;

    for (std::unordered_map<int, Node *>::iterator it = this->nodes.begin(); it != this->nodes.end(); it++)
    {
        int size = it->second->getComponentSize();
        if (size > biggestComponent)
        {
            biggestComponent = size;
        }
    }

    this->biggestComponent = biggestComponent;
    return biggestComponent;
}

void Graph::resetDiameter()
{
    this->diameter = -1;
}

int Graph::getDiameter()
{
    if (this->diameter != -1)
    {
        return this->diameter;
    }

    int max = 0;

    std::vector<Node *> k = this->getBiggestComponentNodes();

    for (std::vector<Node *>::size_type i = 0; i < k.size(); i++)
    {
        int size = k[i]->calculateHighestDistance(k);

        if (size > max)
        {
            max = size;
        }

        for (std::vector<Node *>::size_type i = 0; i < k.size(); i++)
        {
            k[i]->setStatus(Types::UNCHECKED);
        }
    }

    this->diameter = max;

    return max;
}

void Graph::resetRadius()
{
    this->radius = -1;
}

int Graph::getRadius()
{
    if (this->radius != -1)
    {
        return this->radius;
    }

    std::vector<Node *> k = this->getBiggestComponentNodes();

    int min = k[0]->calculateHighestDistance(k);

    for (std::vector<Node *>::size_type i = 1; i < k.size(); i++)
    {
        int size = k[i]->calculateHighestDistance(k);

        if (size < min)
        {
            min = size;
        }

        for (std::vector<Node *>::size_type i = 0; i < k.size(); i++)
        {
            k[i]->setStatus(Types::UNCHECKED);
        }
    }

    this->radius = min;

    return min;
}

void Graph::resetGraph()
{
    for (std::unordered_map<int, Node *>::iterator it = this->nodes.begin(); it != this->nodes.end(); it++)
    {
        it->second->setStatus(Types::UNCHECKED);
    }
}

void Graph::resetGraphFull()
{
    for (std::unordered_map<int, Node *>::iterator it = this->nodes.begin(); it != this->nodes.end(); it++)
    {
        it->second->setStatus(Types::UNCHECKED);
        it->second->setDistance(0);
        it->second->setHighestDistance(-1);
    }
}

std::vector<Node *> Graph::getBiggestComponentNodes()
{
    if (this->biggestComponentNodes.size() != 0)
    {
        return this->biggestComponentNodes;
    }

    std::vector<Node *> nodes;

    std::size_t biggestComponentNodesCount = 0;

    for (std::unordered_map<int, Node *>::iterator it = this->nodes.begin(); it != this->nodes.end(); it++)
    {
        std::vector<Node *> size = it->second->getComponentNodes();
        if (size.size() > biggestComponentNodesCount)
        {
            biggestComponentNodesCount = size.size();
            nodes = size;
        }
    }

    this->biggestComponentNodes = nodes;

    this->resetGraph();

    return nodes;
}