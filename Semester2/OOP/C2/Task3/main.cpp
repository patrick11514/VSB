#include <iostream>
#include <string>
#include "Tree.h"

using namespace std;

Tree *createTree()
{
    Tree *first = (new Tree("Je to minecraft block?"))->setYes("Jedná se o celý block")->setNo("Je to zvíře?");

    // left
    Tree *Ysecond = first->getYes()->setYes("Je složité ho získat?")->setNo("Je tento block ze dřeva?");
    Tree *Ythird = Ysecond->getYes()->setYes("Netherite Block")->setNo("Dá se tento block spálit?");
    Tree *Yfourth = Ythird->getNo()->setYes("Uhelný Block")->setNo("Když ho vykopu, padne ve stejné podobě?");
    Tree *Yfifth = Yfourth->getNo()->setYes("Hlína")->setNo("Stone");

    Tree *Yfourth2 = Ysecond->getNo()->setYes("Dřevěný Slab")->setNo("Je z materiálu, který se dá získat na začátku?");
    Tree *Yfifth2 = Yfourth2->getNo()->setYes("Jedná se o schod?")->setNo("Je tento block z netheru?");
    Tree *Ysixth = Yfifth2->getYes()->setYes("Cobblestone Schody")->setNo("Cobblestone Slab");

    Tree *Yfifth3 = Yfifth2->getNo()->setYes("Nether Brick Fence")->setNo("Jedná se o slab?");
    Tree *Ysixth2 = Yfifth3->getYes()->setYes("Prismarine Block Slab")->setNo("Prismarine Block Stairs");

    // right
    Tree *Nsecond = first->getNo()->setYes("Jedná se o zvíře, které jde ochočit?")->setNo("Jedná se o bosse?");
    Tree *Nthird = Nsecond->getYes()->setYes("Štěká?")->setNo("Má na sobě vlnu?");
    Tree *Nfourth = Nthird->getYes()->setYes("Pes")->setNo("Kočka");

    Tree *Nthird2 = Nsecond->getNo()->setYes("Je v Endu?")->setNo("Minecraft hráč");
    Tree *Nfourth2 = Nthird2->getYes()->setYes("Ender Dragon")->setNo("Wither");

    Tree *Nfourth3 = Nthird->getNo()->setYes("Ovce")->setNo("Je růžové?");
    Tree *Nfifth = Nfourth3->getYes()->setYes("Prase")->setNo("Bučí?");
    Tree *Nsixth = Nfifth->getYes()->setYes("Kráva")->setNo("Slepice");

    return first;
}

int main()
{
    Tree *tree = createTree();
    string answer;

    while (tree->getYes() != nullptr && tree->getNo() != nullptr)
    {
        cout << tree->getQuestion() << " (A/N): ";
        cin >> answer;
        if (tolower(answer[0]) == 'a')
        {
            tree = tree->getYes();
        }
        else if (tolower(answer[0]) == 'n')
        {
            tree = tree->getNo();
        }
        else
        {
            cout << "Neplatná odpověď!" << endl;
        }
    }

    cout << "Tvoje odpověď je: " << tree->getQuestion() << endl;
    return 0;
}