#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//===================== [ TYPES ] ==============
typedef struct
{
    int day;
    char *name;
    float start;
    float end;
    int trades;
} Stock;

typedef struct
{
    Stock **stocks;
    int count;
} Stocks;
//==============================================

//================= [ FUNCTIONS ] ==============

// return pointers for each part of input
char *parseInput(char *input)
{
    return strtok(input, ",\n");
}

// read stocks from input
Stocks *getStocks(int count)
{
    Stocks *stocks = (Stocks *)malloc(sizeof(Stocks));

    if (!stocks)
    {
        printf("Unable to allocate memory\n");
        exit(1);
    }

    stocks->count = count;

    Stock **localStocks = (Stock **)malloc(sizeof(Stock) * count);

    if (!localStocks)
    {
        printf("Unable to allocate memory\n");
        exit(1);
    }

    for (int i = 0; i < count; i++)
    {
        char line[100];
        if (!fgets(line, 50, stdin))
        {
            printf("Unable to read input\n");
            exit(1);
        }

        // parse input
        Stock *stock = malloc(sizeof(Stock));
        if (!stock)
        {
            printf("Unable to allocate memory\n");
            exit(1);
        }
        stock->day = atoi(parseInput(line));

        char *name = parseInput(NULL);
        //+1 for \0
        char *memName = (char *)malloc(sizeof(char) * strlen(name) + 1);

        if (!memName)
        {
            printf("Unable to allocate memory\n");
            exit(1);
        }

        if (!strcpy(memName, name))
        {
            printf("Unable to copy string\n");
            exit(1);
        }

        stock->name = memName;
        stock->start = atof(parseInput(NULL));
        stock->end = atof(parseInput(NULL));
        stock->trades = atoi(parseInput(NULL));

        localStocks[i] = stock;
    }

    stocks->stocks = localStocks;

    return stocks;
}

// print stocks
void printStocks(Stocks *stocks, char *stockName, Stock *stock)
{
    for (int i = stocks->count - 1; i >= 0; i--)
    {
        Stock *s = stocks->stocks[i];

        if (stock != NULL && strcmp(s->name, stockName) == 0)
        {
            printf(
                "<li><b>Day: %i, ticker: %s, start: %.2f, end: %.2f, diff: %.2f, volume: %i</b></li>\n",
                s->day,
                s->name,
                s->start,
                s->end,
                s->end - s->start,
                s->trades);
        }
        else
        {
            printf(
                "<li>Day: %i, ticker: %s, start: %.2f, end: %.2f, diff: %.2f, volume: %i</li>\n",
                s->day,
                s->name,
                s->start,
                s->end,
                s->end - s->start,
                s->trades);
        }
    }
}

// get highest trade for stock if exists
Stock *getHighestTrades(Stocks *stocks, char *name)
{
    int highest = 0;
    Stock *pntr = NULL;

    for (int i = 0; i < stocks->count; i++)
    {
        Stock *stock = stocks->stocks[i];
        if (strcmp(stock->name, name) != 0)
            continue;

        if (stock->trades > highest)
        {
            highest = stock->trades;
            pntr = stock;
        }
    }

    return pntr;
}

// free memory
void freeMem(Stocks *stocks)
{
    for (int i = 0; i < stocks->count; i++)
    {
        Stock *stock = stocks->stocks[i];
        // free name char*
        free(stock->name);
        // free individual Stock*
        free(stock);
    }
    // free Stock**
    free(stocks->stocks);
    // free Stocks*
    free(stocks);
}

// generate SVG graph
void generateGraph(Stocks *stocks, char *stockName)
{
    printf("<svg width=\"100%%\" height=\"auto\">\n<g transform=\"scale(3,6)\">\n");

    const char *green = "#00FF00";
    const char *red = "#FF0000";

    float start = 10;

    float highestDiff = 0.;
    // get diff
    for (int i = 0; i < stocks->count; i++)
    {
        Stock *stock = stocks->stocks[i];
        if (strcmp(stock->name, stockName) == 0)
        {
            float diff = (stock->end - stock->start);
            if (diff < 0)
                diff = diff * -1;

            if (diff > highestDiff)
                highestDiff = diff;
        }
    }
    float lastValue;
    for (lastValue = 0; lastValue < highestDiff; lastValue += 10.)
    {
    }

    for (int i = 0; i < stocks->count; i++)
    {
        Stock *stock = stocks->stocks[i];
        if (strcmp(stock->name, stockName) == 0)
        {
            float diff = (stock->end - stock->start);
            printf("<polyline fill=\"none\" stroke=\"%s\" stroke-width=\"5.000000\" points=\"%f, %f %f, %f\"></polyline>\n", diff > 0 ? green : red, start, lastValue - diff, start, lastValue);
            start += 6;
            lastValue -= diff;
        }
    }

    printf("</g>\n</svg>\n");
}

//==============================================

//=================== [ MAIN ] =================

int main(int argc, char **argv)
{
    // generate graph
    const bool graph = false;

    // constants for html codes
    const char *head = "<html>\n<body>\n<div>\n";
    const char *body = "</div>\n<ul>\n";
    const char *end = "</ul>\n</body>\n</html>\n";

    // check if program got enought arguments
    if (argc < 3)
    {
        printf("Wrong parameters\n");
        return 1;
    }

    // save agruments to variables
    char *stock = argv[1];
    int count = atoi(argv[2]);

    // if yount is 0, stop program
    if (count == 0)
    {
        printf("Zero count\n");
        return 0;
    }

    // get stocks from input
    Stocks *stocks = getStocks(count);

    // get highest trades for stock
    Stock *highestTrades = getHighestTrades(stocks, stock);

    // head of html file
    printf("%s", head);

    // ticker info
    if (highestTrades == NULL)
    {
        printf("Ticker AMC was not found\n");
    }
    else
    {
        printf(
            "<h1>%s: highest volume</h1>\n<div>Day: %i</div>\n<div>Start price: %.2f</div>\n<div>End price: %.2f</div>\n<div>Volume: %i</div>\n",
            highestTrades->name,
            highestTrades->day,
            highestTrades->start,
            highestTrades->end,
            highestTrades->trades);
    }

    // if graph is true and ticker was found, generate it
    if (graph && highestTrades != NULL)
        generateGraph(stocks, stock);

    // print part between ticker info and stocks
    printf("%s", body);

    // print all stocks
    printStocks(stocks, stock, highestTrades);

    // print end of html
    printf("%s", end);

    // free memory
    freeMem(stocks);

    return 0;
}
