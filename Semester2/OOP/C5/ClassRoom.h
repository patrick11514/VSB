#ifndef __ClassRoom_H_
#define __ClassRoom_H_ 1

#include <string>
#include <vector>
#include "Subject.h"
#include "Class.h"

class ClassRoom
{
private:
    string name;
    vector<Subject *> subjects;
    Class *homeClass;

public:
    ClassRoom(string name, Class *homeClass);

    string getName();
    Class *getHomeClass();
    vector<Subject *> getSubjects();

    void setHomeClass(Class *homeClass);
    void setName(string name);
    ClassRoom *addSubject(Subject *subject);
    void removeSubject(Subject *subject);

    void getTimeTable();
};

#endif
