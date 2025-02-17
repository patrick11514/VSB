#ifndef __School_H_
#define __School_H_ 1

#include <string>
#include <vector>
#include "Class.h"
#include "ClassRoom.h"
#include "Student.h"
#include "Teacher.h"
#include "Subject.h"

class School
{
private:
    string name;
    vector<Class *> classes;
    vector<ClassRoom *> classRooms;
    vector<Student *> students;
    vector<Teacher *> teachers;
    vector<Subject *> subjects;

public:
    School(string name);
    ~School();

    string getName();
    vector<Teacher *> getTeachers();
    vector<Class *> getClasses();

    void setName(string name);
    Class *createClass(string name);
    ClassRoom *createClassRoom(string name, Class *homeClass);
    Student *createStudent(string name, string surname);
    Teacher *createTeacher(string name, string surname);
    Subject *createSubject(string name, string from, string to, Teacher *teacher);
};

#endif
