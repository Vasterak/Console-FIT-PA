// THIS IS SOLVED BY RECURSION AND USING MEMOIZATION OTHERWISE TOO SLOW.
// THIS IS A BASICALLY A PROBLEM OF FINDING THE LARGEST POSSIBLE SQUARE IN 2D ARRAY

/* ASSIGNEMENT
Na vstupu jsou (obdobně jako u předchozích zadání) nejdříve zadána 2 celá čísla větší než 0, která určují šířku a výšku pozemku. Dále je zadáno W * H čísel, které vyplní jednotlivé parcely pozemku. Následovně jsou až do signálu EOF dávány dotazy ve tvaru min, kde min je minimální (tedy větší nebo rovno) skóre DEI pro všechny parcely v našem hledaném pozemku.

Úkolem je realizovat program, který tyto vstupy načte a zobrazí největší ČTVERCOVÝ pozemek, který splňuje podmínku viz min a zobrazí souřadnici jeho počátku (tj. jeho levý horní roh).

Pokud takových pozemků existuje více, program vypíše všechny. Pokud takový pozemek neexistuje, program vypíše „Neexistuje.“ (ale nekončí běh).

Chybné vstupy (program po jejich nalezení okamžitě končí svůj běh):

Šířka nebo výška není číslo nebo je menší nebo rovno 0
Skoré DEI parcely není číslo nebo je menší než 0
Min není číslo nebo je menší než 0
Příklad vstupu (> vstup, < výstup):
*/

/*
3 8 2 2
1 4 5 6
7 1 2 3
0 9 1 6

*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/param.h>

typedef struct
{
    size_t width;
    size_t height;
    int ** plotValues;
} Plots;

int largestSquare = 0;

bool _alloc_array(Plots *array)
{
    array->plotValues = (int**) malloc(sizeof(int**) * array->height);
    if (array->plotValues == NULL)
        return false;

    for (size_t i = 0; i < array->height; i++)
    {
        array->plotValues[i] = (int*) malloc(sizeof(int) * array->width);
        if (array->plotValues[i] == NULL)
            return false;
    }
    return true;
}

bool set_dimentions(Plots *array)
{
    int inWidth = 0, inHeight = 0;

    int scanfout = scanf(" %d %d", &inWidth, &inHeight);
    if (scanfout != 2)
    {
        printf("Spatny vstup1\n");
        return false;
    }

    if (inHeight <= 0 || inWidth <= 0)
    {
        printf("Spatny vstup1\n");
        return false;
    }

    array->width = inWidth;
    array->height = inHeight;

    if (!_alloc_array(array))
    {
        printf("allocation failed.\n");
        return false;
    }

    return true;
}

bool set_values(Plots *array)
{
    int inputValue = 0;

    for (size_t i = 0; i < array->height; i++)
    {
        for (size_t j = 0; j < array->width; j++)
        {
            if ((scanf(" %d", &inputValue)) != 1)
            {
                return false;
            }
            array->plotValues[i][j] = inputValue;
        }    
    }
    return true;
}

void print_values(Plots *array)
{
    for (size_t i = 0; i < array->height; i++)
    {
        for (size_t j = 0; j < array->width; j++)
        {
            printf("%d ", array->plotValues[i][j]);            
        }    
        printf("\n");
    }
}

void free_array(Plots *array)
{
    for (size_t i = 0; i < array->height; i++)
    {
        free(array->plotValues[i]);
    }
    free(array->plotValues);
}

int _helper_square(Plots *array, int **cache, size_t xHeight, size_t yWidth, const int minimum)
{
    //check if we are not out of bound this 2d array
    if (xHeight >= array->height || yWidth >= array->width)
        return 0;

    //check if it was NOT already computed and stored in cache.
    if (cache[xHeight][yWidth] == -1)
    {
        int right =     _helper_square(array, cache, xHeight,       yWidth + 1, minimum);
        int diagonal =  _helper_square(array, cache, xHeight + 1,   yWidth + 1, minimum);
        int bottom =    _helper_square(array, cache, xHeight + 1,   yWidth,     minimum);

        //if this local value is >= then set minimum add it to cache.
        if (array->plotValues[xHeight][yWidth] >= minimum)
        {
            cache[xHeight][yWidth] = 1 + MIN(right, MIN(bottom, diagonal));

            //to save computetional power when printing out only largest squares.
            if (cache[xHeight][yWidth] > largestSquare)
                largestSquare = cache[xHeight][yWidth];
        }
        else
            cache[xHeight][yWidth] = 0;
    }

    return cache[xHeight][yWidth];
}

void find_squares(Plots *array, int min)
{
    //create a cache for my data
    int **cache = (int**) calloc(array->height, sizeof(int*));
    for (size_t i = 0; i < array->height; i++)
        cache[i] = (int*) calloc(array->width, sizeof(int));
    
    //set all values to -1 so I know which values were not assigned... As I cannot use NULL
    for (size_t i = 0; i < array->height; i++)
    {
        for (size_t j = 0; j < array->width; j++)
            cache[i][j] = -1;
    }

    _helper_square(array, cache, 0, 0, min);
    //printf("tempShit is: %d\n", tempShit);

    // //PRINT THE CACHE
    // for (size_t i = 0; i < array->height; i++)
    // {
    //     for (size_t j = 0; j < array->width; j++)
    //     {
    //         if (cache[i][j] == -1)
    //             printf("# ");
    //         else
    //             printf("%d ", cache[i][j]);

    //     }
    //     printf("\n");
    // }

    //Print only the biggest possible
    if (largestSquare > 0)
    {
        printf("Nejvetsi: %dx%d\n", largestSquare, largestSquare);

        for (size_t i = 0; i < array->height; i++)
        {
            for (size_t j = 0; j < array->width; j++)
            {
                if (cache[i][j] == largestSquare)
                {
                    printf("* (%ld,%ld)\n", j, i);
                }
            }
        }
    }
    else
        printf("Neexistuje.\n");
    
    //free cache
    largestSquare = 0;
    for (size_t i = 0; i < array->height; i++)
        free(cache[i]);
    free(cache);
}

int main()
{
    Plots pole;

    printf("Zadejte rozmery mapy.\n");

    if (!set_dimentions(&pole))
    {
        printf("Error while reading the input\n");
        return 0;
    }

    printf("Enter the values of each plot\n");

    if (!set_values(&pole))
    {
        printf("Error while reading the input\n");
        return 0;
    }

    printf("Problemy:\n");

    while(true)
    {
        int min;
        int r = scanf(" %d", &min);
        if(r != 1)
        {
            if(r == EOF)
            {
                return 0;
            }
            return 1;
        }
        if(min < 0)
        {
            printf("Bad input value.\n");
            return 1;
        }
        //print_values(&pole);
        find_squares(&pole, min);
    }
    return 0;
    

    print_values(&pole);

    printf("Ending\n");

    free_array(&pole);

    return 0;
}