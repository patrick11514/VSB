#pragma once

#include <vector>

#include "Types.h"

/**
 * @brief Node class
 * Node class, that contains the id, the neighbors and the status of the node
 */
class Node
{
private:
    int id;                        ///< Id of the node
    std::vector<Node *> neighbors; ///< Vector of all neighbors of the node
    int status = Types::UNCHECKED; ///< Status of the node
    int distance = 0;              ///< Distance of the node to the root node
    int highestDistance = -1;      ///< Highest distance of the node to other nodes in component

public:
    /**
     * @brief Construct a new Node object
     * Constructor for the Node class, that takes the id as argument
     *
     * @param id Id of the node
     */
    Node(int id);

    /**
     * @brief Add new neighbor to the node
     * @param neighbor pointer to node that should be added as neighbor
     */
    void addNeighbor(Node *neighbor);
    /**
     * @brief Set status of the node
     * @param status Status of the node
     */
    void setStatus(Types status);

    /**
     * @brief Get the value of "highestDistance"
     * @return int Highest distance of the node to other nodes in component
     */
    int getHighestDistance();
    /**
     * @brief Set the value of "highestDistance"
     * @param distance Highest distance of the node to other nodes in component
     */
    void setHighestDistance(int distance);

    /**
     * @brief Get the value of "status"
     * @return Types Status of the node
     */
    int getDistance();
    /**
     * @brief Set the value of "distance"
     * @param distance Distance of the node to the root node
     */
    void setDistance(int distance);

    /**
     * @brief Get size of component in which the node is
     * @return int Size of component
     */
    int getComponentSize();
    /**
     * @brief Get all nodes in component in which the node is
     * @return std::vector<Node *> Vector of all nodes in component
     */
    std::vector<Node *> getComponentNodes();

    /**
     * @brief Get the id of Node
     * @return int Id of the node
     */
    int getId();
    /**
     * @brief Get the neighbors of Node
     * @return std::vector<Node *> Vector of all neighbors of the node
     */
    std::vector<Node *> getNeighbors();

    /**
     * @brief Calculate the distances of all nodes in component from the current node
     */
    void calculateDistances();
    /**
     * @brief Calculate the highest distance of all nodes in component from the current node
     * @param nodes Vector of all nodes in component
     * @return int Highest distance of the node to other nodes in component
     */
    int calculateHighestDistance(std::vector<Node *> nodes);
};
