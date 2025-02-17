#include <iostream>
#include <string>
#include "KeyValue.h"

using namespace std;

void print(KeyValue *kv)
{
    cout << "Current: Key: " << kv->GetKey() << ", Value: " << kv->GetValue() << endl;
    KeyValue *left = kv->getLeft();
    KeyValue *right = kv->getRight();
    if (left != nullptr)
        print(left);
    if (right != nullptr)
        print(right);
}

void createNext(int count, KeyValue *current, double value)
{
    if (count < 1)
        return;

    if (count % 2 == 0)
    {
        KeyValue *r = current->createRight(count, value + 0.11);
        createNext(count -= 2, r, value += 2.21);
    }
    KeyValue *l = current->createLeft(count + 100, value + 0.56);
    createNext(--count, l, value += 2.29);
}

int main()
{
    int n;
    cout << "Zadejte počet proměnných: ";
    cin >> n;

    double value = 2.89;

    KeyValue *first = new KeyValue(0, value);

    createNext(n, first, value);
    print(first);

    delete first;

    return 0;
}