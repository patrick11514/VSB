#include "Student.h"

Student::Student(string name, string surname)
{
    this->name = name;
    this->surname = surname;
}

string Student::getName()
{
    return this->name;
}

string Student::getSurname()
{
    return this->surname;
}