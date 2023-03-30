#include "Class.h"

Class::Class(string name, Teacher *teacher)
{
    this->name = name;
    this->teacher = teacher;
}

string Class::getName()
{
    return this->name;
}

Teacher *Class::getTeacher()
{
    return this->teacher;
}

void Class::setTeacher(Teacher *teacher)
{
    this->teacher = teacher;
}

void Class::setName(string name)
{
    this->name = name;
}

Class *Class::addStudent(Student *student)
{
    this->students.push_back(student);

    return this;
}

void Class::removeStudent(Student *student)
{
    for (int i = 0; i < this->students.size(); i++)
        if (this->students[i] == student)
        {
            this->students.erase(this->students.begin() + i);
            break;
        }
}
