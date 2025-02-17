#pragma once

#include "Node.h"
#include <unordered_map>

/**
 * @brief Class for the graph
 */
class PKVZGraph
{
private:
    std::unordered_map<std::string, Node *> nodes; ///< The nodes of the graph

public:
    bool initialized = false; ///< If the graph is initialized

    /**
     * @brief Constructs a new PKVZGraph object
     *
     * @param fileName The name of the file with the graph
     */
    PKVZGraph(std::string fileName);

    /**
     * @brief Destroys the PKVZGraph object.
     */
    ~PKVZGraph();

    /**
     * @brief Gets the solution for the graph using DFS algorithm.
     *
     * @param reversed If the solution should be reversed (from the end (|Pkvz) to the start (Pkvz|))
     * @return std::vector<Node*> The solution
     */
    std::vector<Node *> getSolutionDFS(bool reversed) const;
    /**
     * @brief Gets the solution for the graph using BFS algorithm.
     *
     * @param reversed If the solution should be reversed (from the end (|Pkvz) to the start (Pkvz|))
     * @return std::vector<Node*> The solution
     */
    std::vector<Node *> getSolutionBFS(bool reversed) const;
    /**
     * @brief Gets the node with the given status.
     */
    Node *getSpecificNode(std::string status) const;
    /**
     * @brief Resets the nodes of the graph.
     */
    void resetNodes() const;

private:
    /**
     * @brief Gets the start node of the graph.
     *
     * @param reversed If the solution should be reversed (from the end (|Pkvz) to the start (Pkvz|))
     */
    Node *getStart(bool reversed) const;
    /**
     * @brief Performs a cycle in the graph.
     *
     * @param node The node to start the cycle from
     */
    bool performCycle(Node *node) const;
    /**
     * @brief Checks if the node is the end node.
     *
     * @param node The node to be checked
     * @param reverse If the solution should be reversed (from the end (|Pkvz) to the start (Pkvz|))
     */
    bool isEndNode(Node *node, bool reversed) const;
};