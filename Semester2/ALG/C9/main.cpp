#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

/**
 * Reprezentace jedne polozky v ciselniku
 */
struct Item
{
    int key;      ///< Celociselna hodnota, klic polozky
    string value; ///< Nazev nebo jiny retezec popisujici polozku
};

/// Pocet provedenych porovnani mezi prvky v tridenem vektoru
int comparisonCount;
/// Pocet vzajemnych prohozeni prvku v tridenem vektoru
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

/**
 * Implementace algoritmu Lomuto nebo Hoare pro rozdeleni prvku podle pivota.
 * Jako pivot se voli prvek na pozici l ve vektoru items.
 *
 * @param items Vektor polozek, ktere chceme rozdelit
 * @param l Index, od ktereho (vcetne), chceme polozky vektoru delit
 * @param r Index, do ktereho (vcetne), chceme polozky vektoru delit
 * @return Vraci index v poli ukazujici mezi ty dve rozdelenim
 *         ziskane casti pole. V pripade Lomuto to je index, na
 *         kterem skoncil pivot, v pripade Hoare je to index
 *         posledniho prvku mensiho nez pivot.
 */
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

/**
 * Implementace algoritmu Lomuto nebo Hoare pro rozdeleni prvku podle pivota.
 * Jako pivot se voli nahodny prvek vektoru mezi indexy l a r
 * (vcetne techto indexu).
 *
 * @param items Vektor polozek, ktere chceme rozdelit
 * @param l Index, od ktereho (vcetne), chceme polozky vektoru delit
 * @param r Index, do ktereho (vcetne), chceme polozky vektoru delit
 * @return Vraci index v poli ukazujici mezi ty dve rozdelenim
 *         ziskane casti pole. V pripade Lomuto to je index, na
 *         kterem skoncil pivot, v pripade Hoare je to index
 *         posledniho prvku mensiho nez pivot.
 */
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

/**
 * Implementace algoritmu Lomuto nebo Hoare pro rozdeleni prvku podle pivota.
 * Jako pivot se voli median, tedy stredni prvek (podle hodnoty, ne indexu),
 * ze 3 prvku na indexech l, r a (l+r)/2.
 *
 * @param items Vektor polozek, ktere chceme rozdelit
 * @param l Index, od ktereho (vcetne), chceme polozky vektoru delit
 * @param r Index, do ktereho (vcetne), chceme polozky vektoru delit
 * @return Vraci index v poli ukazujici mezi ty dve rozdelenim
 *         ziskane casti pole. V pripade Lomuto to je index, na
 *         kterem skoncil pivot, v pripade Hoare je to index
 *         posledniho prvku mensiho nez pivot.
 */
int Median(vector<Item> &items, int l, int r)
{
    vector<int> keys;

    keys.push_back(items[l].key);
    keys.push_back(items[(r + l) / 2].key);
    keys.push_back(items[r].key);

    sort(keys.begin(), keys.end());

    int pivot = keys[1];

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

/**
 * Implementace tridici funkce Quicksort specialne zamerene na trideni
 * vektoru polozek typu int a s nastavitelnou funkci provadejici rozdeleni
 * prvku podle pivota.
 *
 * @param items Vektor polozek, ktere chceme tridit
 * @param l Index, od ktereho (vcetne), chceme polozky vektoru tridit
 * @param r Index, do ktereho (vcetne), chceme polozky vektoru tridit
 * @param pivot Ukazatel na funkci, ktera provede rozdeleni prvku podle pivota
 *
 * \note
 * Pokud nerozumite ukazatelum na funkce, zkuste nejprve ten parametr pivot
 * ignorovat a volat rovnou funkci First (popr. Random nebo Median). Kdyz
 * vam to bude fungovat, tak jen zmente to "First" na "pivot" a melo by to
 * fungovat dal, ale s tim, ze bude jen 1x napsany Quicksort, ale pri 3 jeho
 * volanich se postupne vyzkousi se vsemi tremi funkcemi: First, Random, Median.
 */
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