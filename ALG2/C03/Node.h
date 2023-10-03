#pragma once

#include <vector>

enum ProblemState
{
    Ok,
    Invalid,
    Neutral
};

enum NodeState
{
    UNVISITED,
    VISITED,
    PROCESSING
};

enum Roles
{
    PASTEVEC,
    KOZA,
    VLK,
    ZELI
};

class Node
{
private:
    ProblemState state;
    NodeState nodeState;
    std::vector<Roles> rightSide;
    std::vector<Roles> leftSide;

public:
    Node();
};