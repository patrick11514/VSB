#include <iostream>
#include <string>
#include <chrono>
#include <vector>

#include "Graph.h"
#include "Node.h"

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    Graph g("test.txt");
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to load graph: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    std::cout << "Biggest component size: " << g.getBiggestComponent() << std::endl;
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to get biggest component: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    g.resetGraph();

    start = std::chrono::high_resolution_clock::now();
    std::vector<Node *> size = g.getBiggestComponentNodes();
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to get biggest component nodes: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    g.resetGraph();

    start = std::chrono::high_resolution_clock::now();
    std::cout << "Biggest component diameter: " << g.getDiameter() << std::endl;
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to get diameter: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    g.resetGraph();

    start = std::chrono::high_resolution_clock::now();
    std::cout << "Biggest component radius: " << g.getRadius() << std::endl;
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to get radius: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    return 0;
}
