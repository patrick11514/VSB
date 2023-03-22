#ifndef _CLIENT_H
#define _CLIENT_H

#include <string>
using namespace std;

class Client
{
private:
    int code;
    string name;

public:
    Client(int c, string n);

    int GetCode();
    string GetName();
};

#endif