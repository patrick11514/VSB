#include "parser.hpp"
#include <iostream>
#include <stack>
#include <string_view>
#include <format>
#include <sstream>

BinaryTree *toTree(std::string input)
{
    if (input.size() == 0)
    {
        return nullptr;
    }

    size_t bracket = input.find('(');
    if (bracket == input.npos)
    {
        return new BinaryTree(std::stoi(input));
    }

    std::string number(input.begin(), input.begin() + bracket);
    BinaryTree *mainTree = new BinaryTree(std::stoi(number));

    std::stack<bool> stack;
    stack.push(true);

    auto position = input.begin() + bracket;

    while (!stack.empty())
    {
        ++position;

        if (*position == ')')
        {
            stack.pop();
        }
        else if (*position == '(')
        {
            stack.push(true);
        }
    }

    mainTree->setLeft(toTree(std::string(input.begin() + bracket + 1, position)));

    std::string_view rightPart(position + 1, input.end());
    if (rightPart.size() > 0)
    {
        mainTree->setRight(toTree(std::string(rightPart.begin() + 1, rightPart.end() - 1)));
    }

    return mainTree;
}

std::string toString(BinaryTree *tree)
{
    std::stringstream str;
    str << std::to_string(tree->getValue());
    if (tree->getLeft() != nullptr)
    {
        str << std::format("({})", toString(tree->getLeft()));
    }

    if (tree->getRight() != nullptr)
    {
        str << std::format("({})", toString(tree->getRight()));
    }
    return str.str();
}
