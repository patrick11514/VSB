#include "tasks.h"

int main()
{
    UTF8String str{std::vector<CodePoint>{
        0x1F601, // 😁
        0x1F614, // 😔
        static_cast<CodePoint>('0'),
        0x010D // č
    }};
    str.append(' ');
    UTF8String str2(str);

    str.print();
    str2.print();
}
