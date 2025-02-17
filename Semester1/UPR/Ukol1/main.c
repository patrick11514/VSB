#include <stdio.h>

void printBank(int value, int count) {
    printf("Bankovka %d: %d\n", value, count);
}

void calculateCount(int bankovka) {

    // 5000
    printBank(5000, bankovka / 5000);
    bankovka = bankovka % 5000;

    // 2000
    printBank(2000, bankovka / 2000);
    bankovka = bankovka % 2000;
    
    // 1000
    printBank(1000, bankovka / 1000);
    bankovka = bankovka % 1000;

    // 500
    printBank(500, bankovka / 500);
    bankovka = bankovka % 500;

    // 200
    printBank(200, bankovka / 200);
    bankovka = bankovka % 200;

    // 100
    printBank(100, bankovka / 100);
    bankovka = bankovka % 100;
}

int main() {
    
    calculateCount(10678);
    
    return 0;
}