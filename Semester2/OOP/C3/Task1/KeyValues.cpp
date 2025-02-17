#include "KeyValues.h"
#include "KeyValue.h"

KeyValues::KeyValues(int n)
{
    this->keyValues = new KeyValue *[n];
    this->count = 0;
}

KeyValues::~KeyValues()
{
    for (int i = 0; i < this->count; i++)
    {
        delete this->keyValues[i];
    }

    delete[] this->keyValues;
}

KeyValue *KeyValues::CreateObject(int k, double v)
{
    KeyValue *obj = new KeyValue(k, v);

    this->keyValues[this->count] = obj;
    this->count += 1;

    return obj;
}

KeyValue *KeyValues::SearchObject(int k)
{
    for (int i = 0; i < this->count; i++)
    {
        if (this->keyValues[i]->GetKey() == k)
        {
            return this->keyValues[i];
        }
    }

    return nullptr;
}

int KeyValues::Count()
{
    return this->count;
}

KeyValue *KeyValues::RemoveObject(int k)
{
    if (this->count - 1 > k && k >= 0)
    {
        KeyValue *obj = this->keyValues[k];
        this->keyValues[k] = nullptr;
        return obj;
    }
    return nullptr;
}