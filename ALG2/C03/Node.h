#pragma once

#include <vector>
#include <string>

/**
 * @brief Enum for the problem status in node
 */
enum ProblemState
{
    OK,       ///< The node is starting/ending point
    INVALID,  ///< The node is invalid
    NEUTRAL,  ///< The node is valid, but not starting/ending point
    UNCHECKED ///< The node is not checked yet
};

/**
 * @brief Enum for the node status
 */
enum NodeState
{
    UNVISITED, ///< The node is not visited yet
    VISITED,   ///< The node is visited
    PROCESSING ///< The node is currently being processed
};

/**
 * @brief Enum for the roles
 */
enum Role
{
    PASTEVEC,
    KOZA,
    VLK,
    ZELI
};

/**
 * @brief Class for the node
 */
class Node
{
private:
    ProblemState state = ProblemState::UNCHECKED; ///< The problem status of the node
    NodeState nodeState = NodeState::UNVISITED;   ///< The node status
    std::vector<Role> rightSide;                  ///< The roles on the right side of the river
    std::vector<Role> leftSide;                   ///< The roles on the left side of the river
    std::vector<Node *> neighbors;                ///< The neighbors of the node

public:
    /**
     * @brief Constructs a new Node object with the given status.
     *
     * @param status The status of the roles on the left and right side of the river splited by '|' character
     */
    Node(std::string status);

    /**
     * @brief Adds a neighbor to the node.
     *
     * @param neighbor The neighbor to be added
     */
    void addNeighbor(Node *neighbor);
    /**
     * @brief Sets the problem status of the node.
     *
     * @param status The problem status to be set
     */
    void setStatus(ProblemState status);
    /**
     * @brief Sets the node status.
     *
     * @param status The node status to be set
     */
    void setNodeStatus(NodeState status);

    /**
     * @brief Gets the problem status of the node.
     *
     * @return ProblemState The problem status of the node
     */
    ProblemState getStatus() const;
    /**
     * @brief Gets the node status.
     *
     * @return NodeState The node status
     */
    NodeState getNodeStatus() const;
    /**
     * @brief Gets the roles on the right side of the river.
     *
     * @return std::vector<Role> The roles on the right side of the river
     */
    std::vector<Role> getRightSide() const;
    /**
     * @brief Gets the roles on the left side of the river.
     *
     * @return std::vector<Role> The roles on the left side of the river
     */
    std::vector<Role> getLeftSide() const;
    /**
     * @brief Gets the neighbors of the node.
     *
     * @return std::vector<Node *> The neighbors of the node
     */
    std::vector<Node *> getNeighbors() const;
    /**
     * @brief Gets if the roles are valid on sides. (No Koza with Zeli, and no Vlk with Koza without Pastevec)
     */
    bool isValid() const;
    /**
     * @brief Gets the status of the roles on the left and right side of the river splited by '|' character.
     *
     * @return std::string The status of the roles on the left and right side of the river splited by '|' character
     */
    std::string getRoleStatus() const;

private:
    /**
     * @brief Gets the role from the character.
     *
     * @param character The character to be converted
     * @return Role The role
     */
    Role getRole(char character) const;
    /**
     * @brief Gets the character from the role.
     *
     * @param role The role to be converted
     * @return char The character
     */
    char getCharacter(Role role) const;
    /**
     * @brief Checks if the roles on the side are valid.
     *
     * @param side The side to be checked
     * @return true The roles on the side are valid
     * @return false The roles on the side are not valid
     */
    bool checkSide(std::vector<Role> side) const;
};