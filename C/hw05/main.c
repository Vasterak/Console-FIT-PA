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

void error_msg()
{
    printf("Nespravny vstup.");
}

char peak_char()
{
    char a = getchar();
    ungetc(a, stdin);
    return a;
}

void clear_whitespaces()
{
    while (isspace(peak_char()))
        getchar();
}

int *realloc_array(int *array, size_t *allocLength)
{
    *allocLength *= 2;
    return (int*) realloc(array, *allocLength * sizeof(int));
}

bool add_stock_price(int **stockPrice, size_t *realLength, size_t *allocLength)
{   
    //DEBUG_PRINT("Pointer address: %p - Pointer value: %p, dereferenced pointer: %p\n", (void *) &stockPrice, (void *) stockPrice, (void *) *stockPrice);
    int userInput = 0;
    // if (scanf(" %d", &userInput) != 1)
    //     return false;

    userInput = *realLength * 10;

    //stock can apperently have a complete zero value
    if (userInput < 0)
        return false;

    if (*realLength > *allocLength - 1)
    {
        DEBUG_PRINT("Realloc happened, address before %p, allocLength: %ld\n", (void *)  *stockPrice, *allocLength);
        *stockPrice = realloc_array(*stockPrice, allocLength);
        if (*stockPrice == NULL)
        {
            free(*stockPrice);
            return false;
        }
        DEBUG_PRINT("Realloc happened, address after %p, allocLength: %ld\n",  (void *) *stockPrice, *allocLength);
    }
    DEBUG_PRINT("Real length: %ld\n", *realLength);
    (*stockPrice)[*realLength] = userInput;
    (*realLength)++;

    return true;
}

int main()
{
    //Initialization of values
    size_t allocLength = DEFAULT_ALLOC;
    size_t realLength = 0;

    //dynamic alloc
    int * stockPrice = (int*) malloc(sizeof(int) * DEFAULT_ALLOC);
    if (stockPrice == NULL)
        return 1;

    printf("Ceny, hledani:\n");

    stockPrice[realLength] = 1;
    realLength++;

    int scanfCodes = 0;
    char operationChar = '\0';
    while (true)
    {
        // scanfCodes = scanf(" %c", &operationChar);
        // if (scanfCodes != 1)
        // {
        //     if (scanfCodes == EOF)
        //         break;
        //     else
        //     {
        //         error_msg();
        //         return 1;
        //     }
        // }

        // switch (operationChar)
        // {
        // case '+':
        //     stockPrice[0] = 100;
        //     //DEBUG_PRINT("Pointer address: %p - Pointer value: %p, dereferenced pointer: %d\n", (void *) &stockPrice, (void *) stockPrice, *stockPrice);
        //     //DEBUG_PRINT("value of first item in array: %d\n", stockPrice[0]);
        //     if (add_stock_price(&stockPrice, &realLength, &allocLength) == false)
        //     {
        //         error_msg();
        //         return 1;
        //     }
        //     break;
            
        // case '?':
        //     break;
        
        // default:
        //     error_msg();
        //     return 1;
        //     break;
        // }

        if (add_stock_price(&stockPrice, &realLength, &allocLength) == false)
        {
            error_msg();
            return 1;
        }

        if (realLength == 10000)
            break;
    }

    free(stockPrice);

    return 0;
}