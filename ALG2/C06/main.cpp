#include <iostream>
#include <string>

#include "BTree.h"
#include "BTreePage.h"

int main()
{
    BTree tree(2);
    for (int i = 0; i < 120; ++i)
    {
        tree.insert(i);
    }

    tree.print();

    std::cout << "Value 10 is in tree? " << (tree.searchValue(10) ? "True" : "False") << std::endl;
    std::cout << "Value 120 is in tree? " << (tree.searchValue(120) ? "True" : "False") << std::endl;
    std::cout << "Count of keys: " << tree.countKeys() << std::endl;
    std::cout << "Capacity: " << tree.capacity() << std::endl;
    std::cout << "Fill factor: " << tree.fillFactor() * 100 << "%" << std::endl;
    std::cout << "Height: " << tree.height() << std::endl;

    return 0;
}
