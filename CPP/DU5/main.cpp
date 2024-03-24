#include "tasks.h"

int main()
{
    std::vector<uint8_t> bytes{1, 2, 3, 4};
    BytesIterator it(bytes.data(), 4);

    for (auto a : it)
    {
        std::cout << static_cast<int>(a) << std::endl;
    }
}
