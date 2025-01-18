#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

int get_random_number(int range)
{
    return rand() % range;
}

void print_array(const int array[], const size_t lengh)
{
    for (size_t i = 0; i < lengh -1; i++)
    {
        printf("%d, ", array[i]);
    }
    printf("%d\n", array[lengh-1]);
}


int main()
{
    //INIT BY CURRENT TIME
    srand(time(NULL));
    int numbers[80];
    size_t numbersLength = sizeof(numbers) / sizeof(int);
    printf("array length: %ld\n", numbersLength);

    //fill up array
    for (size_t i = 0; i < numbersLength; i++)
    {
        numbers[i] = get_random_number(199);
    }

    //print initial values
    print_array(numbers, numbersLength);
    
    //sort them
    for (size_t i = 0; i < numbersLength; i++)
    {
        for (size_t j = 0; j < numbersLength - i -1; j++)
        {
            if (numbers[j] > numbers[j+1])
            {
                int tmp = numbers[j];
                numbers[j] = numbers[j +1];
                numbers[j+1] = tmp;
            }
        }
        
    }
    
    //print array after sorting
    print_array(numbers, numbersLength);

    return 0;
}

// void bubble_sort_op(int array[], size_t length)
// {
//     bool wasSwapped = false;
//     for (size_t i = 0; i < length; i++)
//     {
//         wasSwapped = false;
//         for (size_t j = 0; j < length - i - 1; j++)
//         {
//             if (array[j] > array[j+1])
//             {
//                 wasSwapped = true;
//                 int tmp = array[j];
//                 array[j] = array[j+1];
//                 array[j+1] = tmp;
//             }
//         }
        
//         if (!wasSwapped)
//             break;
//     }
// }