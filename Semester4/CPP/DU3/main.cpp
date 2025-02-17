#include "tasks.h"

void debug(const char *result)
{
    if (result == nullptr)
    {
        std::cout << "nullptr" << std::endl;
    }
    else
    {
        std::cout << result << std::endl;
    }
}

int main()
{
    auto parser = create_word_parser("foo");
    debug(parser("a"));    // null
    debug(parser("f"));    // null
    debug(parser("fo"));   // null
    debug(parser("foox")); // "x"
}
