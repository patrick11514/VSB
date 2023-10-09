#include "Graph.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>

PKVZGraph::PKVZGraph(std::string fileName)
{
    std::ifstream file(fileName);

    // check if file exists
    if (!file.is_open())
    {
        std::cout << "File not found!" << std::endl;
        return;
    }

    std::unordered_map<std::string, Node *> map;

    // read file
    while (!file.eof())
    {
        std::string from, to;
        file >> from >> to;

        if (file.fail())
        {
            std::cout << "Error reading file!" << std::endl;
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
    this->initialized = true;
}

PKVZGraph::~PKVZGraph()
{
    for (auto node : this->nodes)
    {
        delete node.second;
    }
}

Node *PKVZGraph::getSpecificNode(std::string status) const
{
    if (this->nodes.find(status) == this->nodes.end())
    {
        return nullptr;
    }

    return this->nodes.at(status);
}

void PKVZGraph::resetNodes() const
{
    for (auto node : this->nodes)
    {
        node.second->setStatus(ProblemState::UNCHECKED);
        node.second->setNodeStatus(NodeState::UNVISITED);
    }
}

Node *PKVZGraph::getStart(bool reversed) const
{
    if (!reversed)
    {
        for (auto node : this->nodes)
        {
            // if the node has 4 roles on the left side, it is the start
            if (node.second->getLeftSide().size() == 4)
            {
                node.second->setStatus(ProblemState::OK);
                return node.second;
            }
        }
    }
    else
    {
        for (auto node : this->nodes)
        {
            // if the node has 4 roles on the right side, it is the start
            if (node.second->getRightSide().size() == 4)
            {
                node.second->setStatus(ProblemState::OK);
                return node.second;
            }
        }
    }

    return nullptr;
}

bool PKVZGraph::performCycle(Node *node) const
{
    // if node has not been visited and is unchecked
    if (node->getNodeStatus() == NodeState::UNVISITED && node->getStatus() == ProblemState::UNCHECKED)
    {
        // check status
        if (node->isValid())
        {
            // if valid, set status to neutral and add to stack
            node->setStatus(ProblemState::NEUTRAL);
            node->setNodeStatus(NodeState::PROCESSING);
            return true;
        }
        else
        {
            // if invalid, set status to invalid and set node status to visited
            node->setStatus(ProblemState::INVALID);
            node->setNodeStatus(NodeState::VISITED);
        }
    }
    return false;
}

bool PKVZGraph::isEndNode(Node *node, bool reversed) const
{
    // check if node is valid
    if (node->getStatus() == ProblemState::NEUTRAL)
    {
        if (!reversed)
        {
            // check if node is end
            if (node->getRightSide().size() == 4)
            {
                node->setStatus(ProblemState::OK);
                return true;
            }
        }
        else
        {
            // check if node is end
            if (node->getLeftSide().size() == 4)
            {
                node->setStatus(ProblemState::OK);
                return true;
            }
        }
    }
    return false;
}

std::vector<Node *> PKVZGraph::getSolutionDFS(bool reversed) const
{
    std::stack<Node *> stack;
    std::vector<Node *> solution;

    // find the start (all roles will be on same side)
    stack.push(getStart(reversed));

    bool found = false;

    do
    {
        Node *node = stack.top();

        stack.pop();

        for (std::vector<Node *>::size_type i = 0; i < node->getNeighbors().size(); i++)
        {
            if (performCycle(node->getNeighbors()[i]))
            {
                stack.push(node->getNeighbors()[i]);
            }
        }

        if (isEndNode(node, reversed))
        {
            node->setStatus(ProblemState::OK);
            solution.push_back(node);
            found = true;
            break;
        }

        // add node to solution and set node status to visited
        solution.push_back(node);
        node->setNodeStatus(NodeState::VISITED);
    } while (!stack.empty());

    // if no solution found, return empty vector
    if (!found)
    {
        return std::vector<Node *>();
    }

    return solution;
}

std::vector<Node *> PKVZGraph::getSolutionBFS(bool reversed) const
{
    std::queue<Node *> queue;
    std::vector<Node *> solution;

    // find the start (all roles will be on same side)
    queue.push(getStart(reversed));

    bool found = false;

    do
    {
        Node *node = queue.front();

        queue.pop();

        for (std::vector<Node *>::size_type i = 0; i < node->getNeighbors().size(); i++)
        {
            if (performCycle(node->getNeighbors()[i]))
            {
                queue.push(node->getNeighbors()[i]);
            }
        }

        if (isEndNode(node, reversed))
        {
            node->setStatus(ProblemState::OK);
            solution.push_back(node);
            found = true;
            break;
        }

        solution.push_back(node);
        node->setNodeStatus(NodeState::VISITED);
    } while (!queue.empty());

    if (!found)
    {
        return std::vector<Node *>();
    }

    return solution;
}
