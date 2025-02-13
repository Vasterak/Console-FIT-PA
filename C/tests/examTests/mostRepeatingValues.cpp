#include <stdio.h>
#include <stdlib.h>

int cmp_func(const void *x, const void *y)
{
    return *(int*)x - *(int*)y;
}

int main()
{
    size_t arrayLength = 30;
    int *array = (int*) malloc(sizeof(int) * arrayLength);

    for (size_t i = 0; i < arrayLength; i++)
    {
        array[i] = rand() % 8;
    }
    // array[0] = 1;
    // array[1] = 0;
    // array[2] = 1;
    // array[3] = 0;
    

    for (size_t i = 0; i < arrayLength; i++)
    {
        printf("price: %d\n", array[i]);
    }

    qsort(array, arrayLength, sizeof(int), cmp_func);

    printf("\n");
    for (size_t i = 0; i < arrayLength; i++)
    {
        printf("price: %d\n", array[i]);
    }

    //allocate space for the new list
    //Save the most repeating number
    int *saveLongest = (int*) malloc(sizeof(int) * arrayLength);
    size_t saveLongestLength = 1;

    //save the first number
    int currentNumber = array[0];
    saveLongest[0] = currentNumber;
    size_t currentLength = 1;
    size_t bestLength = currentLength;

    for (size_t i = 1; i < arrayLength; i++)
    {
        if (array[i] == currentNumber)
            currentLength++;
        else
        {
            currentLength = 1;
            currentNumber = array[i];
        }
        
        if (currentLength == bestLength)
        {
            saveLongest[saveLongestLength++] = currentNumber;
            //printf("cur %ld == best %ld, curent: %d\n", currentLength, bestLength, currentNumber);
        }

        if (currentLength > bestLength)
        {
            bestLength = currentLength;
            currentNumber = array[i];
            saveLongest[0] = currentNumber;
            saveLongestLength = 1;
        }
    }
    
    printf("List of best\n");
    for (size_t i = 0; i < saveLongestLength; i++)
    {
        printf("price: %d\n", saveLongest[i]);
    }
    
    return 0;
}