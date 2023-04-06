#ifndef _CLIENT_H
#define _CLIENT_H

#include <string>
using namespace std;

class Client
{
private:
    int code;
    string name;

    static int count;

public:
    Client(int c, string n);

    int GetCode();
    string GetName();

    string toString();

    static int getCount()
    {
        return count;
    }
};

#endif