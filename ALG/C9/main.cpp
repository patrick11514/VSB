#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Item
{
    int key;
    string value;
};

int comparisonCount;
int swapCount;

/**
 * Nacteni vstupniho souboru ciselniku do vektoru polozek reprezentovanych
 * strukturami Item, kazda polozka ma celociselny klic (identifikator)
 * a retezcovou hodnotu (nazev)
 * V souboru se ceka kazda polozka na samostatnem radku, na zacatku je
 * klic mezerou oddeleny od hodnoty, ta je potom az do konce radku.
 *
 * @param name Jmeno souboru, ze ktereho se polozky budou nacitat
 * @return Vektor vsech polozek nactenych ze souboru
 */
vector<Item> read_items(string name)
{

    ifstream infile;
    infile.open(name);
    vector<Item> items;
    Item it;

    if (!infile.is_open())
    {
        cerr << "File could not be opened" << endl;
        exit(-1);
    }

    while (!infile.eof())
    {
        infile >> it.key;
        if ((infile.rdstate() & std::ifstream::failbit) != 0)
            continue;
        infile.ignore(256, ' ');
        getline(infile, it.value);
        items.push_back(it);
    }
    infile.close();
    return items;
}

int First(vector<Item> &items, int l, int r)
{
    cout << "L: " << l << endl;
    cout << "R: " << r << endl;
    int pivot = items[l].key;

    int i = l - 1;
    int j = r + 1;

    while (true)
    {
        cout << "BBBBBBBBBBBB" << endl;
        for (vector<Item>::size_type i = 0; i < items.size(); i++)
        {
            cout << items[i].key << " " << items[i].value << endl;
        }

        do
        {
            i++;
        } while (items[i].key < pivot);

        do
        {
            j--;
        } while (items[j].key > pivot);

        if (i >= j)
            return j;

        Item tmp = items[i];
        items[i] = items[j];
        items[j] = tmp;
    }
}

int Random(vector<Item> &items, int l, int r)
{
}

int Median(vector<Item> &items, int l, int r)
{
}

void Quicksort(vector<Item> &items, int l, int r, int (*pivot)(vector<Item> &, int, int))
{
    if (r - l < 1)
        return;

    cout << "========== PRED ==========" << endl;
    for (vector<Item>::size_type i = 0; i < items.size(); i++)
    {
        cout << items[i].key << " " << items[i].value << endl;
    }
    cout << "==============================" << endl;

    int middle = pivot(items, l, r);

    cout << "========== PO ==========" << endl;
    for (vector<Item>::size_type i = 0; i < items.size(); i++)
    {
        cout << items[i].key << " " << items[i].value << endl;
    }
    cout << "==============================" << endl;

    Quicksort(items, l, middle - 1, pivot);
    Quicksort(items, middle + 1, r, pivot);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cerr << "Missing arguments" << endl;
        exit(-1);
    }
    std::srand(std::time(nullptr));

    vector<Item> items;
    items = read_items(argv[1]);
    vector<Item> items2 = items;
    vector<Item> items3 = items;

    comparisonCount = swapCount = 0;
    Quicksort(items, 0, items.size() - 1, First);
    cerr << "First: " << comparisonCount << " porovnani, " << swapCount << " prohozeni" << endl;

    /*comparisonCount = swapCount = 0;
    Quicksort(items2, 0, items2.size() - 1, First);
    cerr << "Random: " << comparisonCount << " porovnani, " << swapCount << " prohozeni" << endl;

    comparisonCount = swapCount = 0;
    Quicksort(items3, 0, items2.size() - 1, First);
    cerr << "Median: " << comparisonCount << " porovnani, " << swapCount << " prohozeni" << endl;*/

    for (unsigned long i = 0; i < items.size(); i++)
    {
        // if (items[i].key == items2[i].key && items[i].key == items3[i].key)
        //{
        cout << items[i].key << " " << items[i].value << endl;
        /*}
        else
        {
            cout << "Vysledky serazeni se lisi" << endl;
            return 0;
        }*/
    }

    return 1;
}