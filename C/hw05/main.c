#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define DEBUG
const int DEFAULT_ALLOC = 1000;

// Macro "function" to toggle debug messages
// https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) do { fprintf(stdout, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__); } while (0)
#else
    #define DEBUG_PRINT(...) do {} while (0)
#endif

void error_msg()
{
    printf("Nespravny vstup.\n");
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
    if (scanf(" %d", &userInput) != 1)
        return false;

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
    (*stockPrice)[*realLength] = userInput;
    (*realLength)++;
    DEBUG_PRINT("RealLength: %ld\n", *realLength);

    return true;
}

bool check_stock(int *stockPrice, size_t *realLength)
{
    //using long long isted of size_t to detect negative values in user input
    long long from, to;
    if (scanf(" %lld %lld", &from, &to) != 2)
        return false;

    if (from < 0 || to < 0 || from > to)
        return false;

    if ((size_t)to >= *realLength)
        return false;

    //main algorithm
    

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
    {
        free(stockPrice);
        return 1;
    }

    printf("Ceny, hledani:\n");

    int scanfCodes = 0;
    char operationChar = '\0';
    while (true)
    {
        scanfCodes = scanf(" %c", &operationChar);
        if (scanfCodes != 1)
        {
            if (scanfCodes == EOF)
                break;
            else
            {
                error_msg();
                return 1;
            }
        }

        switch (operationChar)
        {
        case '+':
            if (!add_stock_price(&stockPrice, &realLength, &allocLength))
            {
                error_msg();
                return 1;
            }
            break;
            
        case '?':
            if (!check_stock(stockPrice, &realLength))
            {
                error_msg();
                return 1;
            }
            break;
        
        default:
            error_msg();
            free(stockPrice);
            return 1;
        }
    }

    for (size_t i = 0; i < realLength; i++)
    {
        printf("%ld: %d\n", i, stockPrice[i]);
    }
    
    free(stockPrice);

    return 0;
}