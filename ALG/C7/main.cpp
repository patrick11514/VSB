#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<string> generateSubSequence(string s, int len)
{
    if (len == 1)
    {
        vector<string> v;
        v.push_back(s);
        return v;
    }
    vector<string> v;
    int strLen = s.length();

    for (int i = 0; i < strLen; i++)
    {
        string str = "";
        str += s[i];

        string toNext = "";
        for (int j = 0; j < strLen; j++)
        {
            if (s[j] != s[i])
            {
                toNext += s[j];
            }
        }

        vector<string> next = generateSubSequence(toNext, len - 1);
        for (vector<string>::size_type j = 0; j < next.size(); j++)
        {
            v.push_back(str + next[j]);
        }
    }

    return v;
}

/**
 * Najde vsechny mozne retezce, ktere lze vytvorit ze znaku
 * vstupniho retezce tak, ze se pouzije kazdy znak prave jednou.
 *
 * @param s Retezec tvoreny znaky, ktere se maji vyskytovat v hledanem retezci (hesle)
 * @param v Ukazatel na vektor, do nehoz funkce prida vsechny mozne kandidaty na hledane heslo
 *
 */
void generate_candidates(string s, vector<string> *v)
{
    vector<string> candidates = generateSubSequence(s, s.length());

    for (vector<string>::size_type i = 0; i < candidates.size(); i++)
    {
        v->push_back(candidates[i]);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cerr << "Missing arguments" << endl;
        exit(-1);
    }
    vector<string> *v = new vector<string>;
    generate_candidates(argv[1], v);
    sort(v->begin(), v->end());
    for (unsigned int i = 1; i <= v->size(); i++)
    {
        cout << (*v)[i - 1] << ((i % 10 == 0) ? '\n' : ' ');
    }
    delete v;
    return 1;
}