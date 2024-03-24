# Assignment
## Improve UTF-8 string
Return to your UTF-8 string from one of the previous lessons and improve it:
- Replace raw pointers with smart pointers
- Add support for the iterator protocol

Make the following changes to the `UTF8String` class:

1) Get rid of manual memory allocation and deallocation and raw pointers.
Use smart pointers or `std::vector` to manage the UTF-8 bytes.
This should considerably simplify the implementation of the string.
2) Add support for move construction and move assignment. If you chose to use a vector, you don't
have to implement any special copy/move constructors, but you can try to do it manually for practice.
3) Add a constructor that receives a vector of UTF-8 bytes.
4) Add support for iterating the UTF-8 bytes of the string.
The UTF-8 byte iterator will be created by a `bytes()` method. The method will return an object
that will support the *iterator protocol* (it will have `begin()` and `end()` methods that will
return iterators). The iterator should support random access.
```
UTF8String str{"hello"};
auto iter = str.bytes().begin();
*iter; // 'h'
iter += 3;
*iter; // 'l'
--iter;
*iter; // 'l'
for (uint8_t ch: str.bytes()) {
   // 'h', 'e', 'l', 'l', 'o'
}
```
5) Add support for iterating the code points of the string.
The code point iterator will be created by a `codepoints()` method. The method will return an object
that will support the *iterator protocol* (it will have `begin()` and `end()` methods that will
return iterators). The iterator should be bidirectional.
```
UTF8String str{std::vector<uint8_t>{
    0xF0, 0x9F, 0x98, 0x81,
    0xF0, 0x9F, 0x98, 0x94,
    0x30,
    0xC4, 0x8D
}};

auto iter = str.codepoints().begin();
++iter;
*iter; // 0x1F614
--iter;
*iter; // 0x1F601

for (CodePoint cp: str.codepoints()) {
    // 0x1F601 '😁'
    // 0x1F614 '😔'
    // '0'
    // 0x010D 'č'
}
```

Implement the two iterators above manually, do not reuse iterators of other STL collections.

## Binary tree
Implement a simple binary tree using smart pointers.

Makes ure to answer questions marked with [*]. You can put the answers in your source code or post
them as Kelvin comments.

Each tree node will contain a value of type `BigData`. Since the value could be quite big,
the value can be shared by some nodes of the tree.
Each node will also contain a left and a right child (both can be possibly missing) and a link
to its parent node (it can also be missing if the current node is the root of the tree).
The parent will be the only owner of its child elements.

[*] What type of pointer should be used for the value and what type should be used for storing
the child nodes? What type of pointer should be used for the parent link? Think about ownership -
who is the owner of what data and which data is shared by whom.

Implement methods for getting the value of a node, its parent and its left/right child.
- `get_parent()`, `bool has_parent()`, `get_left_child()`, `get_right_child()`
Implement method for getting the root node of the tree.
- `get_root()`
Implement methods for taking the left/right child of a node. Taking removes the child from the node and returns it.
- `take_left_child()`, `take_right_child`()
Implement a method for taking a given child of a node. If the given tree is not a child of the node,
throw an exception.
- `take_child(Tree&)`
Implement methods for setting the left/right child of a node. Setting returns the previous child.
- `set_left_child(...)`, `set_right_child(...)`
Implement a method for swapping the left/right child of a node.
- `swap_children()`
Implement a method for finding if two nodes belong to the same tree.
- `bool is_same_tree_as(Tree*)`
Implement a method for replacing the shared value of the current node and of all its descendants
to the passed value.
- `replace_value(...)`

### Bonus task
Implement inorder traversal of the binary tree using the *iterator protocol*.

Example representing this tree:
  0
 / \
1   3
   / \
  2   4

```cpp
auto tree = std::make_unique<Tree>(0);
tree->set_left_child(std::make_unique<Tree>(1));
tree->set_right_child(std::make_unique<Tree>(3));
tree->get_right_child()->set_left_child(std::make_unique<Tree>(2));
tree->get_right_child()->set_right_child(std::make_unique<Tree>(4));

for (auto& node: tree) {
 std::cout << node.get_value() << " ";
}
```
Prints `0, 1, 2, 3, 4`.
