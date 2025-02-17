#include "Teacher.h"

Teacher::Teacher(string name, string surname)
{
    this->name = name;
    this->surname = surname;
}

string Teacher::getName()
{
    return this->name;
}

string Teacher::getSurname()
{
    return this->surname;
}