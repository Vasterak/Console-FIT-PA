#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>


char peekchar()
{
    char a = getchar();
    ungetc(a, stdin);
    return a;
}


int main()
{
    char errorMessage[] = "Nespravny vstup.\n";
    size_t firstNum, secondNum, totalTriangles = 0;
    double c;
    char actionType = ' ';

    printf("Problemy:\n");

    //USER INPUT
    if (scanf(" %c < %lu ; %lu >", &actionType, &firstNum, &secondNum) != 3)
    {
        printf("%s", errorMessage);
        return 0;
    }

    //Check if there is some random char after the user input
    if (!isspace(peekchar()))
    {
        printf("toto tady nema co delat\n");
        printf("%s", errorMessage);
        return 0;
    }

    //Check if first char constains only allowed characters
    if (actionType != '?' && actionType != '#')
    {
        printf("%s", errorMessage);
        return 0;
    }

    //Also check if input numbers are negative, these are not allowed!

    //CALCULATION the pythagoreanTriplets. a,b,c are sites of the triangle
    double tempVariable;

    for (size_t a = firstNum; a < secondNum; a++)
    {
        for (size_t b = a + 1; b < secondNum; b++)
        {
            c = sqrt( a*a + b*b );

            if (c > secondNum)
                break;

            // check if output of sqrt is integer, so then it is a valid right triangle (Pravoúhlý trojúhelník)
            if (modf(c, &tempVariable) == 0)
            {
                printf("* %ld %ld %g\n", a, b, c );
                totalTriangles++;
            }

        }
        
    }

    printf("Celkem: %ld\n", totalTriangles);

    return 0;
}
