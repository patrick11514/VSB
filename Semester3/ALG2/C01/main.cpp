#include <iostream>
#include <string>
#include <vector>
#include <fstream>

typedef struct
{
    std::string word;
    int freq;
} polozka;

int main()
{
    std::vector<polozka> words;

    std::ifstream file;
    file.open("words.txt");

    if (!file.is_open())
    {
        std::cout << "Nepovedlo se otevřít soubor" << std::endl;
        return -1;
    }

    while (!file.eof())
    {
        std::string word;
        file >> word;

        bool exists = false;

        for (std::size_t i = 0; i < words.size(); i++)
        {
            if (words[i].word == word)
            {
                words[i].freq++;
                exists = true;
                break;
            }
        }

        if (!exists)
        {
            polozka item;
            item.freq = 1;
            item.word = word;
            words.push_back(item);
        }
    }

    file.close();

    for (std::size_t i = 0; i < words.size(); i++)
    {
        polozka item = words[i];
        std::cout << item.word << ":	" << item.freq << std::endl;
    }

    return 0;
}
