#ifndef _KEYVALUES_H
#define _KEYVALUES_H

#include <string>
#include "KeyValue.h"
using namespace std;

class KeyValues
{
private:
    KeyValue **keyValues;
    int count;

public:
    KeyValues(int n);
    ~KeyValues();
    KeyValue *CreateObject(int k, double v);
    KeyValue *SearchObject(int key);
    int Count();
    KeyValue *RemoveObject(int k);
};

#endif