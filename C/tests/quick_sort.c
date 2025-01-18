#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define ARRAY_LENGTH 30

int cmp_int(const void *p1, const void *p2)
{
    return *(int *) p1 - *(int *) p2;
    //return strcmp(*(const char **) p1, *(const char **) p2);
}


void print_array(const int array[], const size_t length)
{
    for (size_t i = 0; i < length - 1; i++)
    {
        printf("%d, ", array[i]);
    }
    printf("%d\n", array[length -1]);
    
}

int main()
{
    srand(time(NULL));

    int array[ARRAY_LENGTH];
    size_t arrayLength = ARRAY_LENGTH;

    //fill up array
    for (size_t i = 0; i < ARRAY_LENGTH; i++)
    {
        array[i] = rand() % 50;
    }

    print_array(array, arrayLength);

    qsort(array, arrayLength, sizeof(int), cmp_int);

    print_array(array, arrayLength);

    return 0;
}