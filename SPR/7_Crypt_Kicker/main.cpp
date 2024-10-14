#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using charMap = std::unordered_map<char, char>;

bool solve(std::string &line, std::vector<std::string> &words, charMap &solved,
           size_t begin) {

  std::string cut{line.begin() + begin, line.end()};

  size_t space = cut.find(' ');
  bool last = false;

  if (space == std::string::npos) {
    space = cut.size();
    last = true;
  }

  std::string guessWord{cut.begin(), cut.begin() + space};

  std::string foundWord;

  for (const auto &word : words) {
    if (word.size() != guessWord.size())
      continue;

    // found word with same length

    bool isWrong = false;

    // try to fit already guessed chars, and see, if word is correct
    for (size_t i = 0; i < guessWord.size(); ++i) {
      auto it = solved.find(guessWord[i]);

      if (it == solved.end())
        continue;

      // we found decrypted char, so we compare it with the word
      if (word[i] != it->second) {
        isWrong = true;
        break;
      } // this word is not correct :/
    }

    if (!isWrong) {
      foundWord = word;

      charMap newMap{solved};

      // add new chars
      for (size_t i = 0; i < guessWord.size(); ++i) {
        newMap.emplace(guessWord[i], foundWord[i]);
      }

      // run new iteration

      if (last) {
        solved.swap(newMap);
        return true;
      }

      bool result = solve(line, words, newMap, begin + space + 1);

      if (!result)
        continue;

      // pass the solved map to parent
      solved.swap(newMap);

      return true;
    }
  }
  // we don't find the word, so we must got backward, and try another
  // combination
  return false;
}

void skipNL() {
  std::string s;
  std::getline(std::cin, s);
}

int main() {
  int numberOfWords;

  std::cin >> numberOfWords;
  std::vector<std::string> words;
  words.reserve(numberOfWords);

  skipNL();
  for (int i = 0; i < numberOfWords; ++i) {
    std::string s;
    std::getline(std::cin, s);
    words.emplace_back(s);
  }

  while (true) {

    std::string line;
    std::getline(std::cin, line);

    if (line.size() == 0)
      break;

    charMap charMap;

    bool result = solve(line, words, charMap, 0);
    for (size_t i = 0; i < line.size(); ++i) {
      char c = line[i];

      if (c != ' ') {
        if (result) {
          auto it = charMap.find(c);
          if (it == charMap.end()) {
            std::cout << '*';
          } else {
            std::cout << it->second;
          }
        } else {
          std::cout << '*';
        }
      } else {
        std::cout << ' ';
      }
    }
    std::endl(std::cout);
  }

  return 0;
}
