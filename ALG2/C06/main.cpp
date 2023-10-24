#include <iostream>
#include <string>

#include "BTree.h"
#include "BTreePage.h"

int main()
{
    BTree tree(2);
    tree.insert(2);
    tree.insert(4);
    tree.insert(6);
    tree.insert(10);
    tree.insert(3);
    tree.insert(18);

    std::cout << tree.countKeys() << std::endl;

    // page.print();
    return 0;
}
