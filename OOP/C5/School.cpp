#include "School.h"

School::School(string name)
{
    this->name = name;
}

School::~School()
{
    for (int i = 0; i < this->students.size(); i++)
        delete this->students[i];
    for (int i = 0; i < this->teachers.size(); i++)
        delete this->teachers[i];
    for (int i = 0; i < this->subjects.size(); i++)
        delete this->subjects[i];
    for (int i = 0; i < this->classes.size(); i++)
        delete this->classes[i];
    for (int i = 0; i < this->classRooms.size(); i++)
        delete this->classRooms[i];
}

string School::getName()
{
    return this->name;
}

vector<Teacher *> School::getTeachers()
{
    return this->teachers;
}

vector<Class *> School::getClasses()
{
    return this->classes;
}

void School::setName(string name)
{
    this->name = name;
}

Class *School::createClass(string name)
{
    Class *newClass = new Class(name, nullptr);
    this->classes.push_back(newClass);

    return newClass;
}

ClassRoom *School::createClassRoom(string name, Class *homeClass)
{
    ClassRoom *newClassRoom = new ClassRoom(name, homeClass);
    this->classRooms.push_back(newClassRoom);

    return newClassRoom;
}

Student *School::createStudent(string name, string surname)
{
    Student *newStudent = new Student(name, surname);
    this->students.push_back(newStudent);

    return newStudent;
}

Teacher *School::createTeacher(string name, string surname)
{
    Teacher *newTeacher = new Teacher(name, surname);
    this->teachers.push_back(newTeacher);

    return newTeacher;
}

Subject *School::createSubject(string name, string from, string to, Teacher *teacher)
{
    Subject *newSubject = new Subject(name, from, to, teacher);
    this->subjects.push_back(newSubject);

    return newSubject;
}
