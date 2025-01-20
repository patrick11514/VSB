#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <vector>

#define TIE 0
#define BW 1
#define WW 2

enum Suit { SPADES, HEARTS, DIAMONDS, CLUBS, UNKNOWN };

Suit getSuit(char c) {
  switch (c) {
  case 'C':
    return Suit::CLUBS;
  case 'D':
    return Suit::DIAMONDS;
  case 'H':
    return Suit::HEARTS;
  case 'S':
    return Suit::SPADES;
  }
  return Suit::UNKNOWN;
}

char toSuit(Suit suit) {
  switch (suit) {
  case SPADES:
    return 'S';
  case HEARTS:
    return 'H';
  case DIAMONDS:
    return 'D';
  case CLUBS:
    return 'C';
  case UNKNOWN:
    return 'U';
  }
  return 'U';
}

struct Card {
  Suit suit;
  int value; // 2-10, 11-13: J, Q, K, 14: A

  bool operator<(const Card &right) { return this->value < right.value; }
  bool operator>(const Card &right) { return this->value > right.value; }
  bool operator==(const Card &right) { return this->value == right.value; }
  bool exact(const Card &right) {
    return this->value == right.value && this->suit == right.suit;
  }

  void print() {
    if (this->value < 10) {
      printf("%d%c ", this->value, toSuit(this->suit));
      return;
    }

    char c;

    switch (this->value) {
    case 10:
      c = 'T';
      break;
    case 11:
      c = 'J';
      break;
    case 12:
      c = 'Q';
      break;
    case 13:
      c = 'K';
      break;
    case 14:
      c = 'A';
      break;
    }

    printf("%c%c ", c, toSuit(this->suit));
  }
};

using Cards = std::array<Card, 5>;

static char line[31];

enum Play {
  HIGH_CARDS,
  PAIR,
  TWO_PAIRS,
  THREE_OF_KIND,
  STRAIGHT,
  FLUSH,
  FULL_HOUSE,
  FOUR_OF_KIND,
  STRAIGHT_FLUSH
};

class Hand {
private:
  using Pair = std::pair<int, int>;

  Cards cards;

  std::unordered_map<int, int> counts;
  std::vector<Pair> sortedPairs;

  //
  bool isStraightFlush() {
    Suit suit = cards[0].suit;
    for (int i = 0; i < 5; ++i) {
      if (cards[i].value != cards[0].value + i || cards[i].suit != suit)
        return false;
    }
    return true;
  }
  //
  bool isFourOfKind() {
    if (this->sortedPairs[0].second == 4) {
      return true;
    }
    return false;
  }
  //
  bool isFullHouse() {
    if (this->sortedPairs[0].second == 3 && this->sortedPairs[1].second == 2) {
      return true;
    }
    return false;
  }
  //
  bool isFlush() {
    Suit suit = cards[0].suit;

    for (int i = 1; i < 5; ++i) {
      if (cards[i].suit != suit)
        return false;
    }
    return true;
  }
  //
  bool isStraight() {
    for (int i = 0; i < 5; ++i) {
      if (cards[i].value != cards[0].value + i)
        return false;
    }
    return true;
  }
  //
  bool isThreeOfKind() {
    if (this->sortedPairs[0].second == 3) {
      return true;
    }
    return false;
  }
  //
  bool isTwoPairs() {
    if (this->sortedPairs[0].second == 2 && this->sortedPairs[1].second == 2) {
      return true;
    }
    return false;
  }
  //
  bool isPair() {
    if (this->sortedPairs[0].second == 2) {
      return true;
    }
    return false;
  }
  //

  Play play;

public:
  Hand() {}
  Hand(Cards &cards) : cards(cards) {}

  void print() {
    for (auto &card : cards) {
      card.print();
    }
    putchar('\n');
  }

  void sort() { std::sort(cards.begin(), cards.end()); }

  void getCounts() {
    for (int i = 0; i <= 14 - 2; ++i) {
      counts[i] = 0;
    }

    for (auto &card : cards) {
      counts[card.value - 2]++;
    }

    for (auto &pair : counts) {
      if (pair.second > 0) {
        sortedPairs.push_back(pair);
      }
    }

    std::sort(sortedPairs.begin(), sortedPairs.end(),
              [](Pair &left, Pair &right) {
                // sort card count, then by card value
                if (left.second == right.second) {
                  return left.first > right.first;
                }
                return left.second > right.second;
              });
  }

  void calculateResult() {
    this->sort();
    this->getCounts();

    if (this->isStraightFlush()) {
      this->play = STRAIGHT_FLUSH;
      return;
    }

    if (this->isFourOfKind()) {
      this->play = FOUR_OF_KIND;
      return;
    }

    if (this->isFullHouse()) {
      this->play = FULL_HOUSE;
      return;
    }

    if (this->isFlush()) {
      this->play = FLUSH;
      return;
    }

    if (this->isStraight()) {
      this->play = STRAIGHT;
      return;
    }

    if (this->isThreeOfKind()) {
      this->play = THREE_OF_KIND;
      return;
    }

    if (this->isTwoPairs()) {
      this->play = TWO_PAIRS;
      return;
    }

    if (this->isPair()) {
      this->play = PAIR;
      return;
    }

    this->play = HIGH_CARDS;
  }

  bool operator>(Hand &right) {
    if (this->play > right.play) {
      return true;
    }

    if (this->play < right.play) {
      return false;
    }

    switch (this->play) {
    case HIGH_CARDS:
    case FLUSH:
      for (int i = 4; i >= 0; --i) {
        if (this->cards[i] == right.cards[i])
          continue;

        if (this->cards[i] > right.cards[i]) {
          return true;
        } else {
          return false;
        }
      }
      break;
    case PAIR: {
      int thisPair = this->sortedPairs[0].first;
      int rightPair = right.sortedPairs[0].first;

      if (thisPair > rightPair) {
        return true;
      } else if (rightPair > thisPair) {
        return false;
      }

      // other cards should by only by count of 1 (4 records (pair + 3 by 1))
      for (int i = 1; i < 4; ++i) {
        if (this->sortedPairs[i].first == right.sortedPairs[i].first)
          continue;

        if (this->sortedPairs[i].first > right.sortedPairs[i].first) {
          return true;
        } else {
          return false;
        }
      }

      break;
    }
    case TWO_PAIRS: {
      for (int i = 0; i < 2; ++i) {
        if (this->sortedPairs[i].first == right.sortedPairs[i].first)
          continue;

        if (this->sortedPairs[i].first > right.sortedPairs[i].first) {
          return true;
        } else {
          return false;
        }
      }

      // other card should be only by count of 1 (3 records (2 pairs + 1))

      if (this->sortedPairs[2].first > right.sortedPairs[2].first) {
        return true;
      } else {
        return false;
      }
      break;
    }

    case THREE_OF_KIND:
    case FULL_HOUSE:
    case FOUR_OF_KIND: {
      int thisHighest = this->sortedPairs[0].first;
      int rightHighest = right.sortedPairs[0].first;

      if (thisHighest > rightHighest) {
        return true;
      } else if (rightHighest > thisHighest) {
        return false;
      }

      break;
    }

    case STRAIGHT:
      if (this->cards[4] > right.cards[4]) {
        return true;
      } else {
        return false;
      }
      break;

    case STRAIGHT_FLUSH:
      if (this->cards[4] > right.cards[4]) {
        return true;
      } else {
        return false;
      }
      break;
    }

    return false;
  }

  bool operator<(Hand &right) { return !(*this > right); }

  bool operator==(Hand &right) { return !(*this > right) && !(right > *this); }
};

class Game {
private:
  std::array<Hand, 2> hands;

  void parse() {
    std::array<Cards, 2> hands;

    for (int i = 0; i < 10; ++i) {
      int start = i * 3;

      char card = line[start];
      char suit = line[start + 1];

      int value = 0;

      if (card >= '2' && card <= '9') {
        value = card - '0';
      } else {
        switch (card) {
        case 'T':
          value = 10;
          break;
        case 'J':
          value = 11;
          break;
        case 'Q':
          value = 12;
          break;
        case 'K':
          value = 13;
          break;
        case 'A':
          value = 14;
          break;
        }
      }

      hands[i / 5][i % 5] = Card{getSuit(suit), value};
    }

    this->hands[0] = Hand(hands[0]);
    this->hands[1] = Hand(hands[1]);
  }

public:
  int getResult() {
    this->parse();

    this->hands[0].calculateResult();
    this->hands[1].calculateResult();
    // PlayResult white = this->hands[1].getResult();
    //

    if (this->hands[0] == this->hands[1]) {
      return TIE;
    } else if (this->hands[0] < this->hands[1]) {
      return WW;
    } else {
      return BW;
    }
  }
};

int process() {
  Game game;
  return game.getResult();
}

int main() {
  while (fgets(line, 31, stdin) != NULL) {
    int result = process();
    if (result == BW) {
      printf("Black wins.\n");
    } else if (result == WW) {
      printf("White wins.\n");
    } else {
      printf("Tie.\n");
    }
  }
  return 0;
}
