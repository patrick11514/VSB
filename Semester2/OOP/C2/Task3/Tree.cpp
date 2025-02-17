#include <string>
#include "Tree.h"

using namespace std;

Tree::Tree(string question)
{
    this->question = question;
    this->yes = nullptr;
    this->no = nullptr;
}

Tree::~Tree()
{
    delete yes;
    delete no;
}

Tree *Tree::setYes(string question)
{
    this->yes = new Tree(question);
    return this;
}

Tree *Tree::setNo(string question)
{
    this->no = new Tree(question);
    return this;
}

string Tree::getQuestion()
{
    return this->question;
}

Tree *Tree::getYes()
{
    return this->yes;
}

Tree *Tree::getNo()
{
    return this->no;
}