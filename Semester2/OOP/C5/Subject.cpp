#include "Subject.h"

#include "Teacher.h"

Subject::Subject(string name, string from, string to, Teacher *teacher)
{
    this->name = name;
    this->from = from;
    this->to = to;
    this->teacher = teacher;
}

string Subject::getName()
{
    return this->name;
}

string Subject::getTime()
{
    return this->from + " - " + this->to;
}

Teacher *Subject::getTeacher()
{
    return this->teacher;
}

void Subject::setFrom(string from)
{
    this->from = from;
}

void Subject::setTo(string to)
{
    this->to = to;
}