#ifndef __Class_H_
#define __Class_H_ 1

#include <string>
#include <vector>
#include "Teacher.h"
#include "Student.h"

using namespace std;

class Class
{
private:
    string name;
    Teacher *teacher;
    vector<Student *> students;

public:
    Class(string name, Teacher *teacher);

    string getName();
    Teacher *getTeacher();

    void setTeacher(Teacher *teacher);
    void setName(string name);
    Student *addStudent(Student *student);
    void removeStudent(Student *student);
};

#endif
