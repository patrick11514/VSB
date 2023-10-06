#include "Node.h"

#include <iostream>

Node::Node(std::string status)
{
    bool left = true;
    for (std::size_t i = 0; i < status.size(); i++)
    {
        if (status[i] == '|')
        {
            left = false;
            continue;
        }

        if (left)
        {
            this->leftSide.push_back(getRole(status[i]));
        }
        else
        {
            this->rightSide.push_back(getRole(status[i]));
        }
    }
}

void Node::addNeighbor(Node *neighbor)
{
    this->neighbors.push_back(neighbor);
}

void Node::setStatus(ProblemState status)
{
    this->state = status;
}

void Node::setNodeStatus(NodeState status)
{
    this->nodeState = status;
}

ProblemState Node::getStatus() const
{
    return this->state;
}

NodeState Node::getNodeStatus() const
{
    return this->nodeState;
}

std::vector<Role> Node::getRightSide() const
{
    return this->rightSide;
}

std::vector<Role> Node::getLeftSide() const
{
    return this->leftSide;
}

std::vector<Node *> Node::getNeighbors() const
{
    return this->neighbors;
}

bool Node::isValid() const
{
    if (!checkSide(this->leftSide) || !checkSide(this->rightSide))
    {
        return false;
    }

    return true;
}

std::string Node::getRoleStatus() const
{
    std::string status = "";

    for (std::size_t i = 0; i < this->leftSide.size(); i++)
    {
        status += getCharacter(this->leftSide[i]);
    }

    status += "|";

    for (std::size_t i = 0; i < this->rightSide.size(); i++)
    {
        status += getCharacter(this->rightSide[i]);
    }

    return status;
}

Role Node::getRole(char character) const
{
    switch (character)
    {
    case 'P':
        return Role::PASTEVEC;
    case 'k':
        return Role::KOZA;
    case 'v':
        return Role::VLK;
    default:
        return Role::ZELI;
    }
}

char Node::getCharacter(Role role) const
{
    switch (role)
    {
    case Role::PASTEVEC:
        return 'P';
    case Role::KOZA:
        return 'k';
    case Role::VLK:
        return 'v';
    default:
        return 'z';
    }
}

bool Node::checkSide(std::vector<Role> side) const
{
    bool koza = false;
    bool zeli = false;
    bool vlk = false;
    bool pastevec = false;

    for (std::size_t i = 0; i < side.size(); i++)
    {
        if (side[i] == Role::KOZA)
        {
            koza = true;
        }
        else if (side[i] == Role::ZELI)
        {
            zeli = true;
        }
        else if (side[i] == Role::VLK)
        {
            vlk = true;
        }
        else if (side[i] == Role::PASTEVEC)
        {
            pastevec = true;
        }
    }

    if ((koza && zeli && !pastevec) || (koza && vlk && !pastevec))
    {
        return false;
    }

    return true;
}
