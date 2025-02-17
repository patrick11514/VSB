#include <iostream>
#include <string>

using namespace std;

class Osoba
{
private:
    char *jmeno;
    char *prijmeni;
    int vek;

public:
    Osoba(char *jmeno, char *prijmeni, int vek)
    {
        this->jmeno = jmeno;
        this->prijmeni = prijmeni;
        this->vek = vek;
    }

    void predstaveni()
    {
        cout << "Ahoj, já jsem " << this->jmeno << " " << this->prijmeni << " a mám " << this->vek << " let.\n";
    }

    void ctverec(int delka)
    {
        if (delka < 1)
        {
            cout << "Delka musi byt vetsi nez 0\n";
            return;
        }

        for (int i = 0; i < delka; i++)
        {
            for (int l = 0; l < delka; l++)
            {
                cout << "#";
            }
            cout << "\n";
        }
    }

    void uvolni()
    {
        free(this->jmeno);
        free(this->prijmeni);
    }
};

Osoba *create_osoba()
{
    char *jmeno = (char *)malloc(sizeof(char) * 40);
    char *prijmeni = (char *)malloc(sizeof(char) * 40);
    int vek;

    cout << "Zadej jméno: ";
    cin >> jmeno;
    cout << "Zadej příjmení: ";
    cin >> prijmeni;
    cout << "Zadej věk: ";
    cin >> vek;

    return new Osoba(jmeno, prijmeni, vek);
}

int main()
{

    Osoba *os = create_osoba();
    os->predstaveni();

    os->ctverec(5);

    os->uvolni();
    delete os;
    return 0;
}