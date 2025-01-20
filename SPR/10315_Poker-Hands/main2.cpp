#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
static std::array<Cards, 2> cards;

// 01 34 67
//
void parseCards() {
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

    cards[i / 5][i % 5] = Card{getSuit(suit), value};
  }
}

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

Play getPlay(Cards &cards, int *counts) {
  for (auto &card : cards) {
    counts[card.value - 2]++;
  }

  { ///////////////////////////////////
    // straight flush
    ///////////////////////////////////
    bool same = true;
    for (size_t i = 0; i < cards.size() - 1; ++i) {
      if (cards[i].suit != cards[i + 1].suit) {
        same = false;
        break;
      }
    }

    if (same) {
      bool isOk = true;
      int lastValue = cards[0].value;
      for (size_t i = 1; i < cards.size(); ++i) {
        if (cards[i].value - 1 != lastValue) {
          isOk = false;
          break;
        }
        lastValue = cards[i].value;
      }

      if (isOk)
        return STRAIGHT_FLUSH;
    }
  }
  { ///////////////////////////////////
    // four of kind
    ///////////////////////////////////
    for (int i = 0; i < 13; ++i) {
      if (counts[i] == 4)
        return FOUR_OF_KIND;
    }
  }

  { ///////////////////////////////////
    // full house
    ///////////////////////////////////
    bool two = false;
    bool three = false;
    for (int i = 0; i < 13; ++i) {
      if (counts[i] == 3) {
        three = true;
      } else if (counts[i] == 2) {
        two = true;
      }
      if (three && two)
        return FULL_HOUSE;
    }
  }

  { ///////////////////////////////////
    // flush
    ///////////////////////////////////
    bool same = true;
    Suit lastSuit = cards[0].suit;

    for (size_t i = 1; i < cards.size(); i++) {
      if (cards[i].suit != lastSuit) {
        same = false;
        break;
      }
    }

    if (same)
      return FLUSH;
  }

  { ///////////////////////////////////
    // Straight
    //////////////////////////////////
    bool isOk = true;
    int lastValue = cards[0].value;

    for (size_t i = 1; i < cards.size(); ++i) {
      if (cards[i].value - 1 != lastValue) {
        isOk = false;
        break;
      }
      lastValue = cards[i].value;
    }

    if (isOk)
      return STRAIGHT;
  }
  { ///////////////////////////////////
    // Three of kind
    //////////////////////////////////

    for (int i = 0; i < 13; ++i) {
      if (counts[i] == 3)
        return THREE_OF_KIND;
    }
  }

  { ///////////////////////////////////
    // Two pairs
    //////////////////////////////////
    int pairs = 0;

    for (int i = 0; i < 13; ++i) {
      if (counts[i] == 2) {
        pairs++;
      }

      if (pairs == 2)
        return TWO_PAIRS;
    }
  }

  { ///////////////////////////////////
    // Pair
    //////////////////////////////////
    for (int i = 0; i < 13; ++i) {
      if (counts[i] == 2)
        return PAIR;
    }
  }

  return HIGH_CARDS;
}

std::vector<Card> getRemainingCards(Cards &cards, Play play, int *counts) {
  std::vector<Card> cardsList;

  switch (play) {
  case HIGH_CARDS: {
    return std::vector<Card>{cards.begin(), cards.end()};
  }
  case PAIR: {
    for (auto &card : cards) {
      if (counts[card.value - 2] != 2)
        cardsList.emplace_back(card);
    }
    break;
  }
  case TWO_PAIRS: {
    for (auto &card : cards) {
      if (counts[card.value - 2] != 2) {
        cardsList.emplace_back(card);
      }
    }
    break;
  }
  case THREE_OF_KIND:
  case STRAIGHT:
  case FLUSH:
  case FULL_HOUSE:
  case FOUR_OF_KIND:
  case STRAIGHT_FLUSH:
    break;
  }

  return cardsList;
}

int process() {
  parseCards();

  std::sort(cards[0].begin(), cards[0].end());
  std::sort(cards[1].begin(), cards[1].end());

  int Bcounts[13];
  int Wcounts[13];

  Play black = getPlay(cards[0], Bcounts);
  Play white = getPlay(cards[1], Wcounts);

  if (black > white) {
    return BW;
  }

  if (white > black) {
    return WW;
  }

  Cards BC = cards[0];
  std::vector<Card> BRest = getRemainingCards(BC, black, Bcounts);

  Cards WC = cards[1];
  std::vector<Card> WRest = getRemainingCards(WC, white, Wcounts);

  // handle same
  switch (black) {
  case HIGH_CARDS:
  case FLUSH:
    for (int i = 4; i >= 0; --i) {
      if (BC[i] == WC[i])
        continue;

      if (BC[i] > WC[i]) {
        return BW;
      } else {
        return WW;
      }
    }
    break;
  case PAIR: {
    int BPair = 0;
    for (int i = 0; i < 13; ++i) {
      if (Bcounts[i] == 2) {
        BPair = i;
        break;
      }
    }

    int WPair = 0;
    for (int i = 0; i < 13; ++i) {
      if (Wcounts[i] == 2) {
        WPair = i;
        break;
      }
    }

    if (BPair > WPair)
      return BW;
    else if (WPair > BPair)
      return WW;

    for (int i = 2; i >= 0; --i) {
      if (BRest[i] == WRest[i])
        continue;
      if (BRest[i] > WRest[i]) {
        return BW;
      } else {
        return WW;
      }
    }
    break;
  }
  case TWO_PAIRS: {
    int Bidx = 0;
    int BPairs[2];
    for (int i = 12; i >= 0; --i) {
      if (Bcounts[i] == 2) {
        BPairs[Bidx++] = i;
      }
      if (Bidx == 2)
        break;
    }

    int Widx = 0;
    int WPairs[2];
    for (int i = 12; i >= 0; --i) {
      if (Wcounts[i] == 2) {
        WPairs[Widx++] = i;
      }
      if (Widx == 2)
        break;
    }

    if (BPairs[0] == WPairs[0]) {
      if (BPairs[1] == WPairs[1]) {
        if (BRest[0] == WRest[0])
          break;
        else if (BRest[0] > WRest[0]) {
          return BW;
        } else {
          return WW;
        }
      }

      if (BPairs[1] > WPairs[1]) {
        return BW;
      } else {
        return WW;
      }
    }

    if (BPairs[0] > WPairs[0]) {
      return BW;
    } else {
      return WW;
    }

    break;
  }
  case THREE_OF_KIND: {
    int BType = 0;
    for (int i = 0; i < 13; ++i) {
      if (Bcounts[i] == 3) {
        BType = i;
        break;
      }
    }
    int WType = 0;
    for (int i = 0; i < 13; ++i) {
      if (Wcounts[i] == 3) {
        WType = i;
        break;
      }
    }

    if (BType > WType) {
      return BW;
    } else if (WType > BType) {
      return WW;
    }
    break;
  }

  case STRAIGHT:
  case STRAIGHT_FLUSH:
    if (BC[4] > WC[4])
      return BW;
    else if (WC[4] > BC[4])
      return WW;
    break;
  case FULL_HOUSE: {
    int BType = 0;
    for (int i = 0; i < 13; ++i) {
      if (Bcounts[i] == 3) {
        BType = i;
        break;
      }
    }
    int WType = 0;
    for (int i = 0; i < 13; ++i) {
      if (Wcounts[i] == 3) {
        WType = i;
        break;
      }
    }

    if (BType > WType)
      return BW;
    else if (WType > BType)
      return WW;
    break;
  }

  case FOUR_OF_KIND: {

    int BType = 0;
    for (int i = 0; i < 13; ++i) {
      if (Bcounts[i] == 4) {
        BType = i;
        break;
      }
    }
    int WType = 0;
    for (int i = 0; i < 13; ++i) {
      if (Wcounts[i] == 4) {
        WType = i;
        break;
      }
    }

    if (BType > WType)
      return BW;
    else if (WType > BType)
      return WW;
    break;
  }
  }

  return TIE;
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
