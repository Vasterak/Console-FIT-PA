#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//CONSTANTS
#define ARRAYLIMIT 100000

//VARIABLES
// structure that holds all the carriers
typedef struct carriers
{
    unsigned from;
    unsigned to;
    unsigned packages;
    unsigned price;
} CARRIERS;

//global variables, not great not terrible
CARRIERS listCarriers[ARRAYLIMIT];
unsigned lengthCarriers = 0;
unsigned long long totalPrice = 0;

void error_msg()
{
    printf("Nespravny vstup.\n");
}

bool carriers_input()
{
    
}

int main()
{
    printf("Moznosti dopravy:\n");

    if (!carriers_input())
    {
        error_msg();
        return 0;
    }

        return 0;
}