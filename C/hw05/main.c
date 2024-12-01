#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define DEBUG
const int DEFAULT_ALLOC = 100;

// Macro "function" to toggle debug messages
// https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) do { fprintf(stdout, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__); } while (0)
#else
    #define DEBUG_PRINT(...) do {} while (0)
#endif

int *realloc_array(int *array, size_t *allocLength)
{
    *allocLength *= 2;
    return (int*) realloc(array, *allocLength);
}



int main()
{
    //Initialization of values
    size_t allocLength = DEFAULT_ALLOC;
    size_t realLength = 0;
    int * stockPrice = (int*) malloc(sizeof(int) * DEFAULT_ALLOC);
    if (stockPrice == NULL)
        return 1;

    printf("Ceny, hledani:\n");

    stockPrice[realLength] = 1;
    realLength++;

    while (true)
    {
        stockPrice[realLength] = stockPrice[realLength-1] + 10;
        realLength++;

        if (realLength == allocLength - 1)
        {
            DEBUG_PRINT("%s OLD allocated value: %ld\n","Reallocated!", allocLength);

            stockPrice = realloc_array(stockPrice, &allocLength);

            DEBUG_PRINT("New allocated value: %ld\n", allocLength);
            DEBUG_PRINT("Current length: %ld\n", realLength);

            if (stockPrice == NULL)
                return 1;
        }
        
        if (realLength == 50)
            break;
    }

    free(stockPrice);

    return 0;
}