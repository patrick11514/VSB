#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

/**
 * Reprezentace vrcholu grafu
 */
struct Node
{
    vector<Node *> neighbors; ///< Vektor ukazetelu na sousedni vrcholy
    int id;                   ///< Identifikacni cislo vrcholu

    // Muzete si pridat dalsi polozky, ktere se budou hodit pro reseni ulohy
    // Pokud je chcete mit inicializovane na stejnou hodnotu, staci inicializovat zde, napr.:
    // int tmp = -1;
};

/**
 * Reprezentace grafu
 */
struct Graph
{
    Node *nodes; ///< Pole vrcholu grafu
    int count;   ///< Pocet vrcholu grafu
};

/**
 * Nacte graf ze souboru
 *
 * @param fileName Jmeno souboru, ze ktereho bude graf nacten
 * @return Nacteny graf ve forme struktury Graph
 */

Graph readGraph(string fileName)
{
    int count;
    int from, to;
    ifstream infile;
    infile.open(fileName);

    if (!infile.is_open())
    {
        cerr << "File could not be opened" << endl;
        exit(-1);
    }

    infile >> count;
    Node *nodes = new Node[count];
    Graph graph = {nodes, count};

    for (int i = 0; i < count; i++)
    {
        nodes[i].id = i;
    }
    while (!infile.eof())
    {
        infile >> from >> to;
        if (!infile.fail())
        {
            nodes[from].neighbors.push_back(nodes + to);
            nodes[to].neighbors.push_back(nodes + from);
        }
    }
    infile.close();
    return graph;
}

/**
 * Pomocna funkce, ktera nesmi byt volana ve finalni verzi (neprosly by testy v Kelvinu).
 * Vypise graf v nasledujicim formatu:
 *   - pro kazdy vrchol bude jeden radek
 *   - radek zacina id toho vrcholu a za dvojteckou nasleduje mezerami oddeleny
 *     seznam id vsech jeho sousedu
 *
 * @param graph Graf, ktery bude vypsan
 */
void printGraph(Graph graph)
{
    for (int i = 0; i < graph.count; i++)
    {
        cout << graph.nodes[i].id << ": ";
        for (Node *x : graph.nodes[i].neighbors)
        {
            cout << x->id << " ";
        }
        cout << endl;
    }
}

/**
 * Vypise na standardni vystup vsechny hodnoty z vektoru oddelene mezerami a odradkuje.
 *
 * @param v Vektor, jehoz hodnoty se vypisuji
 */
void printVector(vector<int> v)
{
    for (int i : v)
    {
        cout << i << " ";
    }
    cout << endl;
}

/**
 * Najde vsechny komponenty v grafu. Pro kazdou komponentu secte hodnoty id vrcholu teto
 * komponenty. Vsechny takto ziskane soucty vraci ve forme vektoru.
 *
 * @param graph Graf, ve kterem funkce hleda komponenty
 * @return Vektor, ktery pro kazdou komponentu grafu obsahuje soucet hodnot vrcholu te komponenty
 *
 */
vector<int> findComponents(Graph graph)
{

    // Doplnte telo teto funkce
}

/**
 * V grafu najde vsechny vrcholy, ktere jsou od zadaneho vrcholu presne v zadane vzdalenosti.
 * Vrchol v2 je ve vzdalenosti x od vrcholu v1, jestlize se z v1 da pres x hran dojit k v2
 * a pres mensi pocet hran to nejde.
 *
 * @param graph Graf, ve kterem funkce hleda vrcholy
 * @param node id vrcholu, od ktereho se hleda
 * @param dist Vzdalenost, ve ktere se maji nachazet hledane vrcholy
 * @return Vektor, ktery obsahuje id vsech nalezenych vrcholu
 *
 */
vector<int> findNeighbours(Graph graph, int node, int dist)
{

    // Doplnte telo teto funkce
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        return -1;
    }

    Graph graph;
    vector<int> vect;
    graph = readGraph(argv[1]);
    vect = findComponents(graph);
    sort(vect.begin(), vect.end());
    cout << "Komponenty: ";
    printVector(vect);
    delete[] graph.nodes;

    /* Pro jistotu pred volanim druhe pozadovane funkce se
    cely graf nacte znovu, aby pripadne zmeny, pomocne hodnoty
    atd. provedene/nastavene v grafu volanim prvni funkce nemohly
    ovlivnit vysledek provedeni druhe funkce. */
    graph = readGraph(argv[1]);
    vect = findNeighbours(graph, stoi(argv[2]), stoi(argv[3]));
    sort(vect.begin(), vect.end());
    cout << "Sousede vrcholu " << argv[2] << " ve vzdalenosti " << argv[3] << ": ";
    printVector(vect);
    delete[] graph.nodes;
    return 1;
}