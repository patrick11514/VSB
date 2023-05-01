#include <iostream>
#include <string>
#include <chrono>
#include <vector>

#include "Graph.h"
#include "Node.h"

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    Graph g("TestovaciData/PrumerKomponenty_ShlukovaciKoef_Centralita/Graf3.txt");
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to load graph: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    std::cout << g.getBiggestComponent() << std::endl;
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to get biggest component: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    g.resetGraph();

    start = std::chrono::high_resolution_clock::now();
    std::vector<Node *> size = g.getBiggestComponentNodes();
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to get biggest component nodes: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    g.resetGraph();

    start = std::chrono::high_resolution_clock::now();
    std::cout << g.getDiameter() << std::endl;
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to get diameter: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    g.resetGraph();

    start = std::chrono::high_resolution_clock::now();
    std::cout << g.getRadius() << std::endl;
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to get radius: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    return 0;
}
