#include "Client.h"
#include <string>
using namespace std;

int Client::count = 0;

Client::Client(int c, string n)
{
    this->code = c;
    this->name = n;
    Client::count++;
}

int Client::GetCode()
{
    return this->code;
}
string Client::GetName()
{
    return this->name;
}
string Client::toString()
{
    return "[" + to_string(this->code) + "] " + this->name;
}