#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

int main()
{
    char * errorMessage = "Nespravny vstup.\n";
    int firstNum, secondNum;
    char actionType = ' ';

    printf("Problemy:\n");

    //User input
    if (scanf(" %c < %d ; %d >", &actionType, &firstNum, &secondNum) != 3)
    {
        printf(errorMessage);
        return 0;
    }

    //Check if first char constains only allowed characters
    if (actionType != '?' || actionType != "#")
    {
        printf(errorMessage);
        return 0;
    }

    return 0;
}
