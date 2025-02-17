#pragma once

#include <string>
#include <unordered_map>

#include "Node.h"

/**
 * @brief Graph class
 * Graph class, that contains all the nodes
 */
class Graph
{
private:
    std::unordered_map<int, Node *> nodes;     ///< Map of all nodes in the graph
    std::vector<Node *> biggestComponentNodes; ///< Vector of all nodes in the biggest component

    int biggestComponent = -1; ///< Size of the biggest component
    int diameter = -1;         ///< Diameter of the biggest component
    int radius = -1;           ///< Radius of the biggest component

public:
    bool initialized = false; ///< Whether the graph was initialized correctly

    /**
     * @brief Construct a new Graph object
     * Constructor for the Graph class, that takes the filepath as argument ant tries to load the graph from that file
     * @param fileName Filepath to the graph file
     */
    Graph(std::string fileName);
    /**
     * @brief Destroy the Graph object
     */
    ~Graph();

    /**
     * @brief Reset the value of "biggestComponent" to -1
     */
    void resetBiggestComponent();
    /**
     * @brief Get the Biggest Component object
     * @return int Size of the biggest component
     */
    int getBiggestComponent();

    /**
     * @brief Reset the value of "diameter" to -1
     */
    void resetDiameter();
    /**
     * @brief Get the diameter of biggest component in graph
     * @return int Diameter of the biggest component
     */
    int getDiameter();

    /**
     * @brief Reset the value of "radius" to -1
     */
    void resetRadius();
    /**
     * @brief Get the radius of biggest component in graph
     * @return int Radius of the biggest component
     */
    int getRadius();

    /**
     * @brief Reset the graph
     * Reset the graph by setting all nodes to unvisited
     */
    void resetGraph();
    /**
     * @brief Reset the graph
     * Reset the graph by setting all nodes to unvisited and reseting their distances and highest distances
     */
    void resetGraphFull();

    /**
     * @brief Get nodes of biggest component
     * @return std::vector<Node *> Vector of all nodes in the biggest component
     */
    std::vector<Node *> getBiggestComponentNodes();
};
