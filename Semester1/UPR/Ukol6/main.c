#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// BONUS
int strlen2(char *str)
{
    int i = 0;
    for (i; str[i]; i++)
        ;
    return i;
}

int atoi2(char *str)
{
    int number = 0;

    for (int i = 0; str[i]; i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            number = number * 10 + (str[i] - '0');
        }
    }

    return number;
}

int toLower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return 'a' + (c - 'A');
    else
        return c;
}

int toUpper(char c)
{
    if (c >= 'a' && c <= 'z')
        return 'A' + (c - 'a');
    else
        return c;
}

int isLower(char c)
{
    if (c >= 'a' && c <= 'z')
        return true;
    else
        return false;
}

//=====

void invertWord(char *word, int len)
{
    for (int i = 0; i < len; i++)
    {
        char c = word[i];
        if (isLower(c))
        {
            printf("%c", toUpper(c));
        }
        else
        {
            printf("%c", toLower(c));
        }
    }
}

void wordToSameCase(char *word, int len, bool lower)
{
    for (int i = 0; i < len; i++)
    {
        if (lower)
        {

            printf("%c", toLower(word[i]));
        }
        else
        {

            printf("%c", toUpper(word[i]));
        }
    }
}

int countCase(char *word, int len, bool lower)
{
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if (lower && isLower(word[i]))
            count++;
        else if (!lower && !isLower(word[i]))
            count++;
    }

    return count;
}

int isPalindrome(char *str, int len)
{
    bool is = true;
    for (int i = 0; i <= len / 2; i++)
    {
        if (str[i] != str[len - i - 1])
        {
            is = false;
        }
    }

    return is ? 1 : 0;
}

void processWord(char *word, int len, int currentWord)
{
    // SLOVO N: word invert lc uc lower upper palidrome
    printf("Slovo %d: %s ", currentWord, word);

    // print inverted word
    invertWord(word, len);

    // lower upper
    printf(" %d %d ", countCase(word, len, true), countCase(word, len, false));

    // to lower case
    wordToSameCase(word, len, true);
    printf(" ");
    // to upper case
    wordToSameCase(word, len, false);

    // palindrome
    printf(" %d\n", isPalindrome(word, len));
}

void parseWords(char *row, int len, int *currentWord)
{
    char word[101];

    int l = 0;

    for (int i = 0; i < len; i++, l++)
    {
        if (row[i] == ' ' || i == len - 1)
        {
            int wordLen = l;
            word[l] = '\0';
            l = -1;

            processWord(word, wordLen, *currentWord);

            (*currentWord)++;
        }
        else
        {
            word[l] = row[i];
        }
    }
}

int main()
{
    // 20 ciferne cislo
    char n[21] = {};
    fgets(n, 21, stdin);

    int currentWord = 0;

    for (int i = 0; i < atoi2(n); i++)
    {
        char row[101];
        fgets(row, 101, stdin);

        int len = strlen2(row);

        parseWords(row, len, &currentWord);
    }

    return 0;
}
