#include <iostream>
#include <string>
#include <chrono>
#include <fstream>

#include "BinaryTree.hpp"
#include "parser.hpp"

int main()
{
    std::ifstream file("tree.txt");
    size_t constant = 20000000;
    char *data = (char *)malloc(sizeof(char) * constant);
    file.read(data, constant);
    file.close();

    auto start = std::chrono::high_resolution_clock::now();
    BinaryTree *myTree = toTree(data);

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Time to parse tree: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us" << std::endl;

    start = std::chrono::high_resolution_clock::now();

    std::cout << toString(myTree) << std::endl;

    end = std::chrono::high_resolution_clock::now();

    std::cout << "Time to create string: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us" << std::endl;

    delete myTree;
    free(data);
    return 0;
}
