#include "Client.h"
#include <string>
using namespace std;

Client::Client(int c, string n)
{
    this->code = c;
    this->name = n;
}

int Client::GetCode()
{
    return this->code;
}
string Client::GetName()
{
    return this->name;
}