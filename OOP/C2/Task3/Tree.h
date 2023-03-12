#ifndef _MAIN_H
#define _MAIN_H 1
#include <string>

using namespace std;

class Tree
{
private:
    Tree *yes;
    Tree *no;
    string question;

public:
    // Případně by to šlo přidat to construktoru yes a no argument a bylo by to jednodušší
    Tree(string question);
    ~Tree();
    Tree *setYes(string question);
    Tree *setNo(string question);
    string getQuestion();
    Tree *getYes();
    Tree *getNo();
};

#endif