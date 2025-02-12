#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int factorial(int value)
{
    if (value < 2)
        return 1;
    return value * factorial(value -1);
}

int factorial2(int value)
{
    int finalValue = 1;
    for (size_t i = value; i > 1; i--)
    {
        finalValue *= i;
    }
    return finalValue;
}

int dlazdicky_printing(int targetLength, const int *tiles, char *buffer)
{
    int total = 0;

    //too big for next tile
    if (targetLength < 0)
        return 0;
    //perfect fit
    else if (targetLength == 0)
    {
        //printf the tiles to buffer
        printf("> %s\n", buffer);
        return 1;
    }

    //get the end of this string
    char *tmp = buffer + strlen(buffer);

    for (size_t i = 0; i < 3; i++)
    {
        //write to the ending of string. concate the string
        sprintf(tmp, " %zu", i);
        total += dlazdicky_printing(targetLength - tiles[i] , tiles, buffer);
    }
    
    return total;
}

int dlazdicky_fast(int targetLength, const int *tiles, int *table)
{
    int total = 0;

    //too big for next tile
    if (targetLength < 0)
        return 0;
    //perfect fit
    else if (targetLength == 0)
        return 1;

    if (table[targetLength] == -1)
    {
        for (size_t i = 0; i < 3; i++)
        {
            total += dlazdicky_fast(targetLength - tiles[i] , tiles, table);
        }
        table[targetLength] = total;
    }
    //printf("debug: %d\n", table[targetLength]);
    return table[targetLength];
}

int main()
{
    int input;
    printf("Zadej cislo, jak dlouhy je pasek:\n");
    if (scanf(" %d", &input) != 1)
    {
        printf("Bad input\n");
        return 1;
    }

    int tileTypes[] = {1, 2, 3};
    //char buffer[50] = { '\0'};

    //printf("moznosti je: %d\n", dlazdicky_printing(input, tileTypes, buffer));

    int *table = (int*) malloc(sizeof(int) * input + 1);
    for (int i = 0; i <= input; i++)
    {
        table[i] = -1;
    }
    printf("fast: moznosti je: %d\n", dlazdicky_fast(input, tileTypes, table));


    free(table);
    return 0;
}