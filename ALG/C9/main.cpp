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
    int pivot = items[l].key;

    int i = l - 1;
    int j = r + 1;

    while (true)
    {
        do
        {
            i++;
            comparisonCount++;
        } while (items[i].key < pivot);

        do
        {
            j--;
            comparisonCount++;
        } while (items[j].key > pivot);

        if (i >= j)
            return j;

        swapCount++;
        Item tmp = items[i];
        items[i] = items[j];
        items[j] = tmp;
    }
}

int Random(vector<Item> &items, int l, int r)
{
    int random = l + (rand() % (r - l + 1));

    int pivot = items[random].key;

    int i = l - 1;
    int j = r + 1;

    while (true)
    {
        do
        {
            i++;
            comparisonCount++;
        } while (items[i].key < pivot);

        do
        {
            j--;
            comparisonCount++;
        } while (items[j].key > pivot);

        if (i >= j)
            return j;

        swapCount++;
        Item tmp = items[i];
        items[i] = items[j];
        items[j] = tmp;
    }
}

int Median(vector<Item> &items, int l, int r)
{
    int pivot = items[(r + l) / 2].key;

    int i = l - 1;
    int j = r + 1;

    while (true)
    {
        do
        {
            i++;
            comparisonCount++;
        } while (items[i].key < pivot);

        do
        {
            j--;
            comparisonCount++;
        } while (items[j].key > pivot);

        if (i >= j)
            return j;

        swapCount++;
        Item tmp = items[i];
        items[i] = items[j];
        items[j] = tmp;
    }
}

void Quicksort(vector<Item> &items, int l, int r, int (*pivot)(vector<Item> &, int, int))
{
    if (r - l < 1)
        return;

    int middle = pivot(items, l, r);

    Quicksort(items, l, middle, pivot);
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

    comparisonCount = swapCount = 0;
    Quicksort(items2, 0, items2.size() - 1, Random);
    cerr << "Random: " << comparisonCount << " porovnani, " << swapCount << " prohozeni" << endl;

    comparisonCount = swapCount = 0;
    Quicksort(items3, 0, items2.size() - 1, Median);
    cerr << "Median: " << comparisonCount << " porovnani, " << swapCount << " prohozeni" << endl;

    for (unsigned long i = 0; i < items.size(); i++)
    {
        if (items[i].key == items2[i].key && items[i].key == items3[i].key)
        {
            cout << items[i].key << " " << items[i].value << endl;
        }
        else
        {
            cout << "Vysledky serazeni se lisi" << endl;
            return 0;
        }
    }

    return 1;
}