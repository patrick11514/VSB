#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/// Konstanta s retezcem vypisovanym, pokud polozka nebyla v ciselniku
const string NOT_FOUND = "Neexistujici polozka";

/**
 * Reprezentace jedne polozky v ciselniku
 */
struct Item
{
    int key;      ///< Celociselna hodnota, klic polozky
    string value; ///< Nazev nebo jiny retezec popisujici polozku

    /**
     * Definice operatoru < pro struktury Item, vysledek porovnani zavisi
     * jen na porovnani jejich klicu.
     */
    bool operator<(const Item a) const
    {
        return (key < a.key);
    }
};

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
        infile.ignore(256, ' ');
        getline(infile, it.value);
        items.push_back(it);
    }
    infile.close();
    return items;
}

/**
 * Nacteni seznamu klicu ze souboru do vektoru celociselnych hodnot.
 * V souboru mohou byt cisla oddelena libovolnymi bilymi znaky.
 *
 * @param name Jmeno souboru, ze ktereho se cisla budou nacitat
 * @return Vektor vsech cisel nactenych ze souboru
 */
vector<int> read_queries(string name)
{

    ifstream infile;
    infile.open(name);
    vector<int> queries;
    int i;

    if (!infile.is_open())
    {
        cerr << "File could not be opened" << endl;
        exit(-1);
    }

    while (!infile.eof())
    {
        infile >> i;
        queries.push_back(i);
    }
    infile.close();
    return queries;
}

/**
 * Funkce, ktera pomoci binarniho vyhledavaní (neboli
 * vyhledavani pulenim intervalu) najde v ciselniku nactenem
 * reprezentovanem vektorem polozek typu Item tu s danym klicem
 * a vrati jeji retezcovou hodnotu.
 *
 * @param items Vektor reprezentujici ciselnik
 * @param key Klic, ktery v ciselniku budeme hledat
 * @return Retezcova hodnota (promenna value z polozky Item) polozky
 *         se zadanym klicem
 */
string findItem(vector<Item> &items, int key)
{
    int left = 0;
    int right = items.size() - 1;

    while (true)
    {
        if (left > right)
        {
            return NOT_FOUND;
        }

        int center = (right + left) / 2;

        Item i = items[center];

        if (key == i.key)
        {
            return i.value;
        }
        else if (key < i.key)
        {
            right = center - 1;
        }
        else
        {
            left = center + 1;
        }
    }
}

/**
 * Node of Binary tree reprezentation
 */
class Node
{
    // public values, because I was too lazy to write setters and getters
public:
    int key;
    string value;
    Node *left;
    Node *right;

    /**
     * Constructor
     */
    Node()
    {
        this->left = nullptr;
        this->right = nullptr;
    }

    /**
     * Destructor
     */
    ~Node()
    {
        if (this->left != nullptr)
            delete this->left;
        if (this->right != nullptr)
            delete this->right;
    }

    /**
     * Print node info
     * @param str String, that will be printed after key and value
     */
    void print(string str)
    {
        cout << this->key << "|" << this->value << "|" << str << endl;
        if (this->right != nullptr)
            this->right->print("right");
        if (this->left != nullptr)
            this->left->print("left");
    }
};

/**
 * Class of bianry tree
 */
class Tree
{
private:
    /**
     * Main node
     */
    Node *node;

public:
    /**
     * Contructor
     * @param itesm list of items in binary tree
     */
    Tree(vector<Item> items)
    {
        this->node = new Node();
        this->node->key = items[0].key;
        this->node->value = items[0].value;

        for (vector<Item>::size_type i = 1; i < items.size(); i++)
        {
            bool end = false;

            Item current = items[i];
            Node *currentNode = this->node;

            while (!end)
            {
                if (currentNode->key > current.key)
                {
                    if (currentNode->left == nullptr)
                    {
                        currentNode->left = new Node();
                        currentNode->left->key = current.key;
                        currentNode->left->value = current.value;
                        end = true;
                    }
                    else
                    {
                        currentNode = currentNode->left;
                    }
                }
                else
                {
                    if (currentNode->right == nullptr)
                    {
                        currentNode->right = new Node();
                        currentNode->right->key = current.key;
                        currentNode->right->value = current.value;
                        end = true;
                    }
                    else
                    {
                        currentNode = currentNode->right;
                    }
                }
            }
        }
    }

    /**
     * Destructor
     */
    ~Tree()
    {
        delete this->node;
    }

    /**
     * Search key in binary tree
     * @param key The key, that we are trying to find
     */
    string search(int key)
    {
        Node *currentNode = this->node;

        while (true)
        {
            if (currentNode->key == key)
            {
                return currentNode->value;
            }
            else if (currentNode->key < key)
            {
                if (currentNode->right == nullptr)
                {
                    return NOT_FOUND;
                }
                else
                {
                    currentNode = currentNode->right;
                }
            }
            else
            {
                if (currentNode->left == nullptr)
                {
                    return NOT_FOUND;
                }
                else
                {
                    currentNode = currentNode->left;
                }
            }
        }
    }

    /**
     * Print binary structure
     */
    void print()
    {
        this->node->print("start");
    }
};

int main(int argc, char **argv)
{

    if (argc < 3)
    {
        cerr << "Missing arguments" << endl;
        exit(-1);
    }

    vector<Item> items;
    vector<int> queries;
    items = read_items(argv[1]);
    queries = read_queries(argv[2]);

    /* Nadefinujte si vlastni typ Tree, pokud to nebude trida
     * nebo nebudete mit konstruovani celeho stromu z vektrou
     * items reseno pomoci konstruktoru dane tridy, upravte
     * nasledujici radek
     */
    Tree t(items);
    cout << "Binarni vyhledavaci strom" << endl;
    cout << "-------------------------" << endl;

    // t.print();

    for (int x : queries)
    {
        /* Misto t.search(x) na nasledujicim radku dejte volani
         * vami vytvorene funkce pro vyhledavani v binarnim stromu,
         * predejte ji parametr x obsahujici cele cislo k vyhledani.
         */
        cout << x << " - " << t.search(x) << endl;
    }

    sort(items.begin(), items.end());
    cout << endl;
    cout << "Vyhledavani pulenim intervalu" << endl;
    cout << "-----------------------------" << endl;
    for (int x : queries)
    {
        cout << x << " - " << findItem(items, x) << endl;
    }
    return 1;
}
