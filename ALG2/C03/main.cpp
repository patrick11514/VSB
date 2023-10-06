#include <iostream>
#include <string>
#include "Graph.h"

int main()
{
    std::string fileName = "graph.txt";

    PKVZGraph g(fileName);

    if (!g.initialized)
    {
        std::cout << "Graph not initialized!" << std::endl;
        return 1;
    }

    std::cout << "DFS solution:" << std::endl;

    std::vector<Node *> solutionDFS = g.getSolutionDFS(false);

    if (solutionDFS.empty())
    {
        std::cout << "No solution for DFS found!" << std::endl;
        return 1;
    }

    for (auto node : solutionDFS)
    {
        std::cout << node->getRoleStatus() << std::endl;
    }

    std::cout << std::endl;
    g.resetNodes();

    std::cout << "BFS solution:" << std::endl;

    std::vector<Node *> solutionBFS = g.getSolutionBFS(false);

    if (solutionBFS.empty())
    {
        std::cout << "No solution for BFS found!" << std::endl;
        return 1;
    }

    for (auto node : solutionBFS)
    {
        std::cout << node->getRoleStatus() << std::endl;
    }

    return 0;
}
