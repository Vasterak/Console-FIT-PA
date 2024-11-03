#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>

const char g_errorMessage[] = "Nespravny vstup.\n";

// char peekchar()
// {
//     char a = getchar();
//     ungetc(a, stdin);
//     return a;
// }

// 0 == USER INPUT ERROR, 1 == PRINT TRIANGLE VALUES, 2 == PRINT ONLY TOTAL NUMBER OF TRIANGLES
int user_input(size_t *firstNum, size_t *secondNum, char *actionType)
{
    // if (scanf(" %c < %lu ; %lu >", &actionType, firstNum, secondNum) != 3)
    //     return 0;

    if (*firstNum < 0 || *secondNum < 0 || *secondNum < *firstNum)
        return 0;

    // Check if there is some random char after the user input
    // if (!isspace(peekchar()))
    // {
    //     printf("toto tady nema co delat\n");
    //     return 0;
    // }

    // Check if first char constains only allowed characters
    if (*actionType != '?' && *actionType != '#')
    {
        return 0;
    }

    if (*actionType == '?')
        return 1;
    else
        return 2;
}

void calculate_loop(size_t *firstNum, size_t *secondNum, double *c, size_t *totalTriangles, char actionType)
{
    // CALCULATION
    // a,b,c are sites of the triangle
    // pythagorean Triplets

    double tempVariable;
    for (size_t a = *firstNum; a < *secondNum; a++)
    {
        for (size_t b = a + 1; b < *secondNum; b++)
        {
            *(c) = sqrt(a * a + b * b);

            if (*c > *secondNum)
                break;

            // check if output of sqrt is integer, so then it is a valid right triangle (Pravoúhlý trojúhelník)
            if (modf(*(c), &tempVariable) == 0)
            {
                if (actionType == '?')
                    printf("* %ld %ld %g\n", a, b, *c);

                (*totalTriangles)++;
            }
        }
    }
}

int main()
{
    //VARIABLES
    size_t firstNum = 0, secondNum = 0, totalTriangles = 0;
    double c;
    char actionType = ' ';

    //START OF THE PROGRAM
    printf("Problemy:\n");

    while (true)
    {
        int inputType = scanf(" %c < %lu ; %lu >", &actionType, &firstNum, &secondNum);

        if (inputType == EOF )
            return 0;

        if (inputType != 3)
        {
            //printf("1\n");
            printf("%s", g_errorMessage);
            return 0;
        }

        switch (user_input(&firstNum, &secondNum, &actionType))
        {
        case 0:
            //printf("2\n");
            printf("%s", g_errorMessage);
            return 0;

        case 1:
            calculate_loop(&firstNum, &secondNum, &c, &totalTriangles, '?');
            break;

        case 2:
            calculate_loop(&firstNum, &secondNum, &c, &totalTriangles, '#');
            break;

        default:
            return 0;
        }

        printf("Celkem: %ld\n", totalTriangles);
        totalTriangles = 0;
    }
    
    return 0;
}
