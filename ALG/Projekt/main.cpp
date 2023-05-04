#include <iostream>
#include <string>
#include <chrono>
#include <vector>

#include "Graph.h"
#include "Node.h"

int main()
{
    auto mainStart = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::high_resolution_clock::now();
    Graph g("TestovaciData/PrumerKomponenty_ShlukovaciKoef_Centralita/Graf1.txt");

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
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to get biggest component nodes: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
*/

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
