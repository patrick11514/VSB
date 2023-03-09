#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <stack>
#include <cctype>
using namespace std;

const int N = 35;
string TagType[N] = {"a", "b", "big", "body", "center", "code", "dir", "div", "head", "h1",
                     "h2", "h3", "h4", "h5", "h6", "html", "i", "li", "ol", "pre",
                     "style", "small", "s", "span", "sub", "sup", "table", "tbody", "td", "thead",
                     "title", "tr", "tt", "u", "ul"};

enum errors
{
    OK,
    NotOpened,
    NotMatching,
    NotClosed
};

struct Tag
{
    bool open;     // Je oteviraci
    int tt;        // Typ tagu (poradi v poli TagType)
    int line;      // Radek vyskytu
    int character; // Znak v ramci radku, kde se vyskytuje
};

queue<Tag> read(string fileName)
{
    ifstream infile;
    queue<Tag> fronta;
    bool open = true;
    char c;
    int lc = 1;        // line count
    int cc = 0;        // char count
    int tagStartC = 0; // tag start character (column)
    int tagStartL = 0; // tag start line
    bool tagStarted = false;
    string s = "";

    infile.open(fileName);
    if (!infile.is_open())
    {
        cerr << "File could not be opened" << endl;
        exit(-1);
    }

    while (infile.get(c))
    {
        cc++;
        if (c == '<')
        {
            tagStarted = true;
            tagStartC = cc;
            tagStartL = lc;
            continue;
        }
        if (c == '/' && tagStarted)
        {
            open = false;
            continue;
        }
        if (tagStarted && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
        {
            s += tolower(c);
        }
        if ((c == '>' || c == ' ') && tagStarted)
        {
            for (int i = 0; i < N; i++)
            {
                if (s == TagType[i])
                {
                    fronta.push({open, i, tagStartL, tagStartC});
                    break;
                }
            }
            s.clear();
            tagStarted = false;
            open = true;
        }
        if (c == '\n')
        {
            lc++;
            cc = 0;
            continue;
        }
    }
    infile.close();
    return fronta;
}

void print(Tag b)
{
    cout << b.line << "." << b.character << ": <" << ((b.open) ? "" : "/") << TagType[b.tt] << ">" << endl;
}

void printQ(queue<Tag> fronta)
{
    Tag tag;
    while (!fronta.empty())
    {
        tag = fronta.front();
        fronta.pop();
        print(tag);
    }
}

void error(int e, Tag t1 = {false, 0, 0, 0}, Tag t2 = {false, 0, 0, 0})
{
    switch (e)
    {
    case OK:
        cout << "HTML is OK" << endl;
        break;
    case NotOpened:
        cout << t1.line << "." << t1.character << ": closing tag </" << TagType[t1.tt] << ">, all is already closed" << endl;
        break;
    case NotMatching:
        cout << t1.line << "." << t1.character << ": closing tag </" << TagType[t1.tt] << ">, last oppened is " << t2.line << "." << t2.character << ": <" << TagType[t2.tt] << ">" << endl;
        break;
    case NotClosed:
        cout << t1.line << "." << t1.character << ": opening tag </" << TagType[t1.tt] << "> is not closed" << endl;
        break;
    }
}

bool check(queue<Tag> queue)
{
    stack<Tag> stack;
    Tag current;
    Tag fromStack;
    while (!queue.empty())
    {
        current = queue.front();
        queue.pop();
        if (current.open)
        {
            stack.push(current);
        }
        else
        {
            if (stack.empty())
            {
                error(NotOpened, current);
                return false;
            }

            fromStack = stack.top();
            if (fromStack.tt != current.tt)
            {
                error(NotMatching, current, fromStack);
                return false;
            }
            stack.pop();
        }
    }

    if (!stack.empty())
    {
        error(NotClosed, stack.top());
        return false;
    }
    error(OK);
    return true;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        cerr << "Add file name as command line argument" << endl;
        return -1;
    }
    return check(read(argv[1]));
}
