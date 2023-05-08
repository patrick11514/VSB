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

        size++;

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

void Node::calculateDistances()
{
    std::queue<Node *> nodes;
    this->status = Types::CHECKING;
    this->distance = 0;
    nodes.push(this);

    do
    {
        Node *node = nodes.front();
        nodes.pop();

        for (std::vector<Node *>::size_type i = 0; i < node->neighbors.size(); i++)
        {
            if (node->neighbors[i]->status == Types::UNCHECKED)
            {
                nodes.push(node->neighbors[i]);
                node->neighbors[i]->status = Types::CHECKING;
                node->neighbors[i]->distance = node->distance + 1;
            }
        }

        node->status = Types::CHECKED;

    } while (!nodes.empty());
}

int Node::calculateHighestDistance(std::vector<Node *> nodes)
{
    if (nodes.size() == 0)
    {
        return 0;
    }

    if (this->highestDistance != -1)
    {
        return this->highestDistance;
    }

    int highestDistance = 0;

    this->calculateDistances();

    for (std::vector<Node *>::size_type i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->id == this->id)
        {
            continue;
        }

        int distance = nodes[i]->distance;

        if (distance > highestDistance)
        {
            highestDistance = distance;
        }
    }

    this->highestDistance = highestDistance;

    return highestDistance;
}

int Node::getHighestDistance()
{
    return this->highestDistance;
}

void Node::setHighestDistance(int distance)
{
    this->highestDistance = distance;
}

int Node::getDistance()
{
    return this->distance;
}

void Node::setDistance(int distance)
{
    this->distance = distance;
}