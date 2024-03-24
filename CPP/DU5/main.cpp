#include "tasks.h"

std::unique_ptr<Tree> christmas_tree()
{
    /*
     * tree:
     *               1
     *              / \
     *             /   \
     *            /     \
     *           2       5
     *          / \     / \
     *         3   4   6   7
     *        /             \
     *       8               9
     */
    auto tree = std::make_unique<Tree>(1);
    tree->set_left_child(std::make_unique<Tree>(2));
    tree->get_left_child()->set_left_child(std::make_unique<Tree>(3));
    tree->get_left_child()->set_right_child(std::make_unique<Tree>(4));
    tree->get_left_child()->get_left_child()->set_left_child(std::make_unique<Tree>(8));

    tree->set_right_child(std::make_unique<Tree>(5));
    tree->get_right_child()->set_left_child(std::make_unique<Tree>(6));
    tree->get_right_child()->set_right_child(std::make_unique<Tree>(7));
    tree->get_right_child()->get_right_child()->set_right_child(std::make_unique<Tree>(9));
    return tree;
}

int main()
{
    auto tree = christmas_tree();

    std::vector<int> values;
    for (const auto &node : *tree)
    {
        values.push_back(node.get_value().value);
    }

    // std::cout << (values == std::vector<int>{8, 3, 2, 4, 1, 6, 5, 7, 9}) << std::endl;

    for (auto &a : values)
    {
        std::cout << a << std::endl;
    }
}
