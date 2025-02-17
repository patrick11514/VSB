#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
    char jmeno[15];
    char prijmeni[15];
    int rokNar;
    float vsp;
} Student;

void studentPrint(Student s)
{
    printf("%s %s se narodil v roce %d a ma prumer %f\n", s.jmeno, s.prijmeni, s.rokNar, s.vsp);
}

void studentEdit(Student *s)
{
    s->rokNar = 2000;
}

Student studentGet()
{
    char buffer[50];
    fgets(buffer, 50, stdin);
    char *prijmeni = strtok(buffer, ",");
    char *jmeno = strtok(NULL, ",");
    int rok = atoi(strtok(NULL, ","));
    float vsp = atof(strtok(NULL, "\n"));

    Student s;
    strcpy(s.jmeno, jmeno);
    strcpy(s.prijmeni, prijmeni);
    s.rokNar = rok;
    s.vsp = vsp;
    return s;
}

int main()
{
    Student s = studentGet();

    studentPrint(s);

    return 0;
}
