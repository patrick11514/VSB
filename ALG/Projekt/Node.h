#ifndef __Node_H_
#define __Node_H_ 1

#include <vector>

#include "Types.h"

class Node
{
private:
    int id;
    std::vector<Node *> neighbors;
    int status = Types::UNCHECKED;

public:
    Node(int id);

    void addNeighbor(Node *neighbor);
    int getComponentSize();

    int getId();
    std::vector<Node *> getNeighbors();
};

#endif
