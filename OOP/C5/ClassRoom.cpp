#include "ClassRoom.h"

#include <string>
#include <vector>
#include <iostream>
#include "Subject.h"
#include "Class.h"

using namespace std;

ClassRoom::ClassRoom(string name, Class *homeClass)
{
    this->name = name;
    this->homeClass = homeClass;
}

string ClassRoom::getName()
{
    return this->name;
}

Class *ClassRoom::getHomeClass()
{
    return this->homeClass;
}

vector<Subject *> ClassRoom::getSubjects()
{
    return this->subjects;
}

ClassRoom *ClassRoom::addSubject(Subject *subject)
{
    this->subjects.push_back(subject);

    return this;
}
void ClassRoom::removeSubject(Subject *subject)
{
    for (int i = 0; i < this->subjects.size(); i++)
    {
        if (this->subjects[i]->getName() == subject->getName())
        {
            this->subjects.erase(this->subjects.begin() + i);
        }
    }
}

void ClassRoom::getTimeTable()
{
    cout << "Time table for " << this->name << " class room" << endl;

    for (int i = 0; i < this->subjects.size(); i++)
    {
        cout << this->subjects[i]->getTime() << " "
             << this->subjects[i]->getName() << " - "
             << this->subjects[i]->getTeacher()->getName() << " "
             << this->subjects[i]->getTeacher()->getSurname() << endl;
    }
}