#include <vector>
#include <stack>

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

/*void Node::getComponentSize()
{
    int size = 0;

    stack<Node *> stack;
    stack.push(node);
    node->status = CHECKING;

    do
    {
        Node *node = stack.top();
        stack.pop();

        size += node->id;

        for (vector<Node *>::size_type i = 0; i < node->neighbors.size(); i++)
        {
            if (node->neighbors[i]->status == UNCHECKED)
            {
                stack.push(node->neighbors[i]);
                node->neighbors[i]->status = CHECKING;
            }
        }

        node->status = CHECKED;
    } while (!stack.empty());

    return size;
}*/

std::vector<Node *> Node::getNeighbors()
{
    return this->neighbors;
}