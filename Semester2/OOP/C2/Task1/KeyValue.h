#ifndef _KEY_VALUE_H
#define _KEY_VALUE_H 1
class KeyValue
{
private:
    int key;
    double value;
    KeyValue *next;

public:
    // constructor
    KeyValue(int k, double v);
    // destructor
    ~KeyValue();
    // get key
    int GetKey();
    // get value
    double GetValue();
    // create next key value and return it
    KeyValue *createNext(int k, double v);
    // get next key value
    KeyValue *getNext();
};
#endif