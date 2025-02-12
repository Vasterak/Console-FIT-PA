// poprosil bych kdyžtak o překontrolování ještě někým
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
 
// načte velikost, tou naplní len, následně parcely uloží do 1D pole a to vrátí
// informace o pozicích parcel si vůbec pamatovat nemusíme
long long* getPrices(int* len)
{
 
    int sizeX = -1, sizeY = -1;
    int res = scanf("%d %d", &sizeX, &sizeY);
    if (res != 2) {return NULL;}
    *len = sizeX * sizeY;
 
    long long* prices = (long long*) malloc(*len * (sizeof(*prices)));
    for (int i = 0; i < *len; i++)
    {
        long long next;
        res = scanf("%lld", &next);
        if (res != 1) {return NULL;}
        prices[i] = next;
    }
    return prices;
 
}
 
// argumenty a návratová hodnota musí být přesně těchto typů, aby to s qsort() fungovalo
int cmpll(const void* a, const void* b)
{
    // do písmene okopírované z cppreference, jen s long long místo intů
    long long arg1 = *(const long long*)a;
    long long arg2 = *(const long long*)b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}
 
long long calculateTax(int purchased, const long long tax, const long long withoutTax)
{
 
    if (purchased < withoutTax) {return 0;}
    return (purchased+1 - withoutTax) * tax;
}
 
// zpracuje a vypíše jeden dotaz
void processPurchase(long long* prices, int len, long long money, long long tax, long long withoutTax)
{
    int purchased = 0;
 
    // prochází parcely od nejlevnější po nejdražší
    for (int i = 0; i < len; i++)
    {
        // daň pro tuto iteraci
        long long currentTax = calculateTax(purchased, tax, withoutTax);
 
        // pokud si nemůžeme už dovolit další parcelu
        if (prices[i] + currentTax > money)
        {
            printf("Pozemky: %d, zbylo: %lld\n", purchased, money); return;
        }
        money -= prices[i];
        money -= currentTax;
        purchased++;
    }
    // pokud jsme koupili všechny zadané parcely
    printf("Pozemky: %d, zbylo: %lld\n", purchased, money); return;
 
}
 
 
 
// trochu zkrácený zápis pro zpřehlednění
#define wrongInput() {puts("Nespravny vstup."); return __LINE__;}
 
int main()
{
    assert(calculateTax(0, 20, 2) == 0);
    assert(calculateTax(1, 20, 2) == 0);
    assert(calculateTax(2, 20, 2) == 20);
    assert(calculateTax(3, 20, 2) == 40);
 
 
    puts("Mapa:");
    int len = -1;
    long long* prices = getPrices(&len);
 
    // parcely si seřadíme od nejmenší po největší
    // poslední argument je pointer na funkci, docela fujky věc
    qsort(prices, len, sizeof(*prices), cmpll);
 
 
    puts("Investice:");
    while(1)
    {
        char firstChar = 0;
 
        int res = scanf(" %c", &firstChar);
        if (res == EOF) break;
        if (firstChar != '?') wrongInput();
 
        long long money = -1, tax = -1, withoutTax = -1;
        res = scanf("%lld %lld %lld", &money, &tax, &withoutTax);
        if (res == EOF) break;
        if (money < 0)  wrongInput();
 
        if (res == 1)
        {
            processPurchase(prices, len, money, 0, 0); continue;
        }
 
        if (res == 3)
        {
            if (tax < 1)        wrongInput();
            if (withoutTax < 0) wrongInput();
            processPurchase(prices, len, money, tax, withoutTax); continue;
        }
    }
    free(prices);
}