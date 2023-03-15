#include <iostream>
#include <string>
#include "KeyValue.h"
#include "KeyValues.h"

int main()
{
    int N = 5;
    KeyValues *values = new KeyValues(N);

    KeyValue *keyValue = values->CreateObject(0, 0.5);
    cout << keyValue->GetValue() << endl;

    for (int i = 1; i < N; i++)
    {
        values->CreateObject(i, i + 0.5);
    }

    cout << values->SearchObject(4)->GetValue() << endl;

    delete values;
    return 0;
}