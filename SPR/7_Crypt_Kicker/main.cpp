#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using charMap = std::unordered_map<char, char>;
using wordMap = std::unordered_map<std::string, std::vector<std::string>>;

std::string getHash(const std::string &word) {
  std::string hash;
  std::unordered_map<char, int> numToInt;
  int i = 0;

  for (const char c : word) {
    auto find = numToInt.find(c);
    if (find == numToInt.end()) {
      numToInt.emplace(c, i);
      ++i;
      hash.push_back('0' + i);
      continue;
    }

    hash.push_back('0' + find->second);
  }

  return hash;
}

bool solve(std::string &line, wordMap &wordMap, charMap &solved, size_t begin) {

  std::string cut{line.begin() + begin, line.end()};

  size_t space = cut.find(' ');
  bool last = false;

  if (space == std::string::npos) {
    space = cut.size();
    last = true;
  }

  std::string guessWord{cut.begin(), cut.begin() + space};

  std::string hash = getHash(guessWord);

  auto it = wordMap.find(hash);

  // no map found
  if (it == wordMap.end()) {
    return false;
  }

  for (const auto &word : it->second) {
    bool isWrong = false;

    charMap newMap{solved};

    // try to fit already guessed chars, and see, if word is correct
    for (size_t i = 0; i < guessWord.size(); ++i) {
      auto it = newMap.find(guessWord[i]);

      if (it == newMap.end()) {
        newMap.emplace(guessWord[i], word[i]);
        continue;
      }

      // we found decrypted char, so we compare it with the word
      if (word[i] != it->second) {
        isWrong = true;
        break;
      } // this word is not correct :/
    }

    if (!isWrong) {
      // if last
      if (last) {
        solved.swap(newMap);
        return true;
      }

      bool result = solve(line, wordMap, newMap, begin + space + 1);

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
    wordMap wordMap;

    for (const auto &word : words) {
      std::string hash = getHash(word);
      auto it = wordMap.find(hash);
      if (it == wordMap.end()) {
        wordMap.emplace(hash, std::vector<std::string>{word});
        continue;
      }
      it->second.emplace_back(word);
    }

    bool result = solve(line, wordMap, charMap, 0);
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
