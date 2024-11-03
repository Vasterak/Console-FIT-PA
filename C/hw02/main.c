#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>

const char g_errorMessage[] = "Nespravny vstup.\n";

char peekchar()
{
    char a = getchar();
    ungetc(a, stdin);
    return a;
}

void remove_whiteSpace()
{
    while (isspace(peekchar()))
        getchar();
}

// Function to find the Greatest Common Divisor of two numbers
int gcd(int a, int b)
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 0 == USER INPUT ERROR, 1 == PRINT TRIANGLE VALUES, 2 == PRINT ONLY TOTAL NUMBER OF TRIANGLES
int user_input(int *firstNum, int *secondNum, char *actionType)
{
    // if (scanf(" %c < %lu ; %lu >", &actionType, firstNum, secondNum) != 3)
    //     return 0;

    if (*firstNum < 0 || *secondNum < 0 || *firstNum > *secondNum)
        return 0;

    if (*firstNum == 0 || *secondNum == 0)
        return 0;

    //Check if after the last number there is accualy >
    remove_whiteSpace();
    if (peekchar() != '>' )
    {
        // printf("tady fail\n");
        // printf("znak byl: %c\n", peekchar());
        return 0;
    }
    else
        getchar(); // remove the '>' from stdin

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

void calculate_loop(int *firstNum, int *secondNum, double *c, int *totalTriangles)
{
    // CALCULATION
    // a,b,c are sites of the triangle
    // pythagorean Triplets

    double tempVariable;
    for (int a = *firstNum; a < *secondNum; a++)
    {
        for (int b = a + 1; b < *secondNum; b++)
        {
            *(c) = sqrt(a * a + b * b);

            if (*c > *secondNum)
                break;

            // check if output of sqrt is integer, so then it is a valid right triangle (Pravoúhlý trojúhelník)
            if (modf(*(c), &tempVariable) == 0)
            {
                printf("* %d %d %g\n", a, b, *c);

                (*totalTriangles)++;
            }
        }
    }
}

//optimalization for speed
void calculate_loop_opt(int *firstNum, int *secondNum, int *totalTriangles)
{
    int coefficient, a, b, c;
    int primeA, primeB, primeC;
    for (int m = 2; m * m <= *secondNum; m++)
    {
        for (int n = 1; n < m; n++)
        {
            // Check if m and n are coprime and have opposite parity
            if ((m - n) % 2 == 1 && gcd(m, n) == 1)
            {
                // Generate the primitive triple
                primeA = m * m - n * n;
                primeB = 2 * m * n;
                primeC = m * m + n * n;

                // Find all multiples of primitives 
                coefficient = 1;
                while (1)
                {
                    a = coefficient * primeA;
                    b = coefficient * primeB;
                    c = coefficient * primeC;

                    // Stop if c exceeds the secondNum aka limit
                    if (c > *secondNum)
                        break;

                    coefficient++;
                    if (a > *firstNum - 1 && b > *firstNum - 1 && c > *firstNum - 1)
                    {
                        (*totalTriangles)++;
                    }
                }
            }
        }
    }
}

int main()
{
    //VARIABLES
    int firstNum = 0, secondNum = 0, totalTriangles = 0;
    double c;
    char actionType = ' ';

    //START OF THE PROGRAM
    printf("Problemy:\n");

    while (true)
    {
        int inputType = scanf(" %c < %d ; %d", &actionType, &firstNum, &secondNum);

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
            calculate_loop(&firstNum, &secondNum, &c, &totalTriangles);
            break;

        case 2:
            calculate_loop_opt(&firstNum, &secondNum, &totalTriangles);
            break;

        default:
            return 0;
        }

        printf("Celkem: %d\n", totalTriangles);
        totalTriangles = 0;
    }
    
    return 0;
}
