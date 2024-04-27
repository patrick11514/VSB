#include "tasks.h"

int main()
{
    using Program = Stack<PushNumber<6>, PushNumber<6>, Compare>;
    Interpret<Program, Stack<>>::Result;
}
