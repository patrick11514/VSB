#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/**
 * Reprezentace jedné polozky urcene k preprave
 */
struct item
{
    int weight; ///< Vaha polozky
    int price;  ///< Cena za prepravu teto polozky
};

/**
 * Nacteni vstupniho souboru do vektoru obsahujiciho pro kazdou polozku
 * (coz odpovida radku ve vstupnim souboru) dvojici celociselnych hodnot
 * vaha a cena
 *
 * @param name Jmeno souboru, ze ktereho se polozky budou nacitat
 * @return Vektor vsech polozek nactenych ze souboru
 */
vector<item> read_file(string name)
{

    ifstream infile;
    infile.open(name);
    vector<item> items;
    item it;

    if (!infile.is_open())
    {
        cerr << "File could not be opened" << endl;
        exit(-1);
    }

    while (!infile.eof())
    {
        infile >> it.weight >> it.price;
        items.push_back(it);
    }
    infile.close();
    return items;
}

/**
 * Najde maximalni mozny soucet cen produktu takovy,
 * ze soucet vah polozek do nej zapoctenych je
 * mensi nebo povolene nosnosti.
 *
 * @param items Vektor polozek, mezi nimiz hledame tu maximalni sadu
 * @param max_load Povolena nosnost - maximalni mozny soucet vah vybranych produktu
 * @return Nalezena maximalni mozna cena pri splneni podminek
 *
 */
int find_max(vector<item> items, int max_load)
{
    vector<bool> subset;

    int max = 0;

    // Fill with false
    for (vector<item>::size_type i = 0; i < items.size(); i++)
    {
        subset.push_back(false);
    }

    vector<bool>::size_type vecSize = subset.size();

    int sets = (1 << items.size()) - 1;

    for (int i = 0; i < sets; i++)
    {
        int j = 0;
        while (subset[j] == true)
        {
            subset[j] = false;
            j++;
        }

        subset[j] = true;

        int load = 0;
        int price = 0;

        for (vector<bool>::size_type i = 0; i < vecSize; i++)
        {
            if (subset[i] == true)
            {
                load += items[i].weight;
                price += items[i].price;
            }
        }

        if (load <= max_load)
        {
            if (price > max)
            {
                max = price;
            }
        }
    }

    return max;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "Missing arguments" << endl;
        exit(-1);
    }

    cout << find_max(read_file(argv[1]), stoi(argv[2])) << endl;
    return 1;
}