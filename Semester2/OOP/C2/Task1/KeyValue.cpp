#include "KeyValue.h"
KeyValue::KeyValue(int k, double v)
{
    this->key = k;
    this->value = v;
}
KeyValue::~KeyValue()
{
    if (this->next != nullptr)
    {
        delete this->next;
        this->next = nullptr;
    }
}

int KeyValue::GetKey()
{
    return this->key;
}

double KeyValue::GetValue()
{
    return this->value;
}

KeyValue *KeyValue::createNext(int k, double v)
{
    this->next = new KeyValue(k, v);
    return this->next;
}

KeyValue *KeyValue::getNext()
{
    return this->next;
}