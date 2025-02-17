#include "KeyValue.h"
KeyValue::KeyValue(int k, double v)
{
    this->key = k;
    this->value = v;
}
KeyValue::~KeyValue()
{
    if (this->left != nullptr)
    {
        delete this->left;
        this->left = nullptr;
    }
    if (this->right != nullptr)
    {
        delete this->right;
        this->right = nullptr;
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

KeyValue *KeyValue::createLeft(int k, double v)
{
    this->left = new KeyValue(k, v);
    return this->left;
}

KeyValue *KeyValue::createRight(int k, double v)
{
    this->right = new KeyValue(k, v);
    return this->right;
}

KeyValue *KeyValue::getLeft()
{
    return this->left;
}

KeyValue *KeyValue::getRight()
{
    return this->right;
}