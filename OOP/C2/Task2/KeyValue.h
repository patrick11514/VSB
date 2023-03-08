#ifndef _KEY_VALUE_H
#define _KEY_VALUE_H 1
class KeyValue
{
private:
    int key;
    double value;
    KeyValue *left;
    KeyValue *right;

public:
    // constructor
    KeyValue(int k, double v);
    // destructor
    ~KeyValue();
    // get key
    int GetKey();
    // get value
    double GetValue();
    // create left key value and return it
    KeyValue *createLeft(int k, double v);
    // create right key value and return it
    KeyValue *createRight(int k, double v);
    // get left key value
    KeyValue *getLeft();
    // get right key value
    KeyValue *getRight();
};
#endif