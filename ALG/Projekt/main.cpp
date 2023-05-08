/**
 * @author Patrik Mintěl
 * @date 4.5.2023
 * @version 0.2.0
 */

#include <iostream>
#include <string>
#include <chrono>
#include <vector>

#include "Graph.h"
#include "Node.h"

int main(int argc, char **args)
{
    if (argc < 2)
    {
        std::cout << "Enter file path to graph as argument" << std::endl;
        return 1;
    }
    auto mainStart = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::high_resolution_clock::now();
    Graph g(args[1]);

    if (!g.initialized)
    {
        std::cout << "Could not initialize graph" << std::endl;
        return 1;
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Time to load graph: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    std::cout << "Biggest component size: " << g.getBiggestComponent() << std::endl;
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to get biggest component: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    g.resetGraph();

    /*start = std::chrono::high_resolution_clock::now();
    std::vector<Node *> size = g.getBiggestComponentNodes();
    std::cout << "Biggest component nodes: " << size.size() << std::endl;
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to get biggest component nodes: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;*/

    start = std::chrono::high_resolution_clock::now();
    int radius = g.getRadius();
    std::cout << "Biggest component radius: " << radius << std::endl;
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to get radius: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    int diameter = g.getDiameter();
    std::cout << "Biggest component diameter: " << diameter << std::endl;
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to get diameter: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    g.resetGraph();

    auto mainEnd = std::chrono::high_resolution_clock::now();

    std::cout << "Sum Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(mainEnd - mainStart).count() << " ms" << std::endl;

    return 0;
}
