#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <chrono>

#include "Node.h"
#include "Types.h"

Node::Node(int id)
{
    this->id = id;
}

void Node::addNeighbor(Node *neighbor)
{
    this->neighbors.push_back(neighbor);
}

int Node::getId()
{
    return this->id;
}

int Node::getComponentSize()
{
    if (this->status == Types::CHECKED)
        return 0;

    int size = 0;

    std::stack<Node *> stack;
    stack.push(this);
    this->status = Types::CHECKING;

    do
    {
        Node *node = stack.top();
        stack.pop();

        size += node->id;

        for (std::vector<Node *>::size_type i = 0; i < node->neighbors.size(); i++)
        {
            if (node->neighbors[i]->status == Types::UNCHECKED)
            {
                stack.push(node->neighbors[i]);
                node->neighbors[i]->status = Types::CHECKING;
            }
        }

        node->status = Types::CHECKED;
    } while (!stack.empty());

    return size;
}

std::vector<Node *> Node::getComponentNodes()
{
    std::vector<Node *> nodes;

    if (this->status == Types::CHECKED)
        return nodes;

    std::stack<Node *> stack;
    stack.push(this);
    nodes.push_back(this);
    this->status = Types::CHECKING;

    do
    {
        Node *node = stack.top();
        stack.pop();

        for (std::vector<Node *>::size_type i = 0; i < node->neighbors.size(); i++)
        {
            if (node->neighbors[i]->status == Types::UNCHECKED)
            {
                stack.push(node->neighbors[i]);
                nodes.push_back(node->neighbors[i]);
                node->neighbors[i]->status = Types::CHECKING;
            }
        }

        node->status = Types::CHECKED;
    } while (!stack.empty());

    return nodes;
}

std::vector<Node *> Node::getNeighbors()
{
    return this->neighbors;
}

void Node::setStatus(Types status)
{
    this->status = status;
}

int Node::getDistance(Node *fNode)
{
    std::queue<Node *> nodes;
    std::vector<Node *> visited;
    this->status = Types::CHECKING;
    this->distance = 0;
    nodes.push(this);
    visited.push_back(this);

    do
    {
        Node *node = nodes.front();
        nodes.pop();

        if (fNode->id == node->id)
        {
            for (std::vector<Node *>::size_type i = 0; i < visited.size(); i++)
            {
                visited[i]->status = Types::UNCHECKED;
            }

            return node->distance;
        }

        for (std::vector<Node *>::size_type i = 0; i < node->neighbors.size(); i++)
        {
            if (node->neighbors[i]->status == Types::UNCHECKED)
            {
                nodes.push(node->neighbors[i]);
                visited.push_back(node->neighbors[i]);
                node->neighbors[i]->status = Types::CHECKING;
                node->neighbors[i]->distance = node->distance + 1;
            }
        }

        node->status = Types::CHECKED;

    } while (!nodes.empty());

    return -1;
}

int Node::getHighestDistance(std::vector<Node *> nodes)
{
    if (nodes.size() == 0)
    {
        return 0;
    }

    int highestDistance = 0;

    for (std::vector<Node *>::size_type i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->id == this->id)
        {
            continue;
        }

        int distance = this->getDistance(nodes[i]);

        if (distance > highestDistance)
        {
            highestDistance = distance;
        }
    }

    return highestDistance;
}