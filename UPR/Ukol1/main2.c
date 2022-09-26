#include <stdio.h>

void printBank(int value, int count) {
    printf("Bankovka %d: %d\n", value, count);
}

void calculateCount(int bankovka) {

    int banknotes[] = {5000, 2000, 1000, 500, 200, 200};

    for (int i = 0; i < sizeof(banknotes) / sizeof(banknotes[0]); i++)
    {
        printBank(banknotes[i], bankovka / banknotes[i]);
        bankovka = bankovka % banknotes[i];
    }
}

int main() {
    
    calculateCount(10678);
    
    return 0;
}