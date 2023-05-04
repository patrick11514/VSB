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
    int distance = 0;
    int highestDistance = -1;

public:
    Node(int id);

    void addNeighbor(Node *neighbor);
    void setStatus(Types status);

    int getHighestDistance();
    void setHighestDistance(int distance);

    int getDistance();
    void setDistance(int distance);

    int getComponentSize();
    std::vector<Node *> getComponentNodes();

    int getId();
    std::vector<Node *> getNeighbors();

    void calculateDistances();
    int calculateHighestDistance(std::vector<Node *> nodes);
};

#endif
