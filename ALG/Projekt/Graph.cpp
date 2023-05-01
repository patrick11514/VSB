#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "Graph.h"
#include "Node.h"

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
}

Graph::~Graph()
{
    for (std::unordered_map<int, Node *>::iterator it = this->nodes.begin(); it != this->nodes.end(); it++)
    {
        delete it->second;
    }
}
