#include <iostream>
#include <string>
#include "KeyValue.h"

using namespace std;

void printAll(KeyValue *first)
{
    KeyValue *next = first;
    do
    {
        cout << "Key: " << next->GetKey() << ", Value: " << next->GetValue() << endl;
        next = next->getNext();
    } while (next != nullptr);
}

int main()
{
    int n;
    cout << "Zadejte počet proměnných: ";
    cin >> n;

    if (n < 1)
    {
        cout << "Zadejte číslo větší než 0." << endl;
        return 1;
    }

    double value;

    cout << "Zadejte první desetinné číslo: ";
    cin >> value;

    KeyValue *first = new KeyValue(0, value);
    KeyValue *next = first;

    for (int i = 1; i < n; i++)
    {
        cout << "Zadejte další desetinné číslo: ";
        cin >> value;

        next = next->createNext(i, value);
    }

    printAll(first);
    delete first;

    return 0;
}