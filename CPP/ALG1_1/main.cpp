#include <iostream>
#include <string>
#include <chrono>

#include "BinaryTree.hpp"
#include "parser.hpp"

int main()
{

    auto start = std::chrono::high_resolution_clock::now();
    BinaryTree *myTree = toTree("3(1(2(4(8(16)(17))(9(18)(19)))(5(10)(11(22)(23))))(3(6(12)(13))(7(14)(15))))(1(2(4(8)(9))(5(10)(11)))(3(6(12)(13))(7(14)(15))))");

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Time to parse tree: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us" << std::endl;

    start = std::chrono::high_resolution_clock::now();

    std::cout << toString(myTree) << std::endl;

    end = std::chrono::high_resolution_clock::now();

    std::cout << "Time to create string: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us" << std::endl;

    delete myTree;
    return 0;
}
