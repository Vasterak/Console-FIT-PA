#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// bool check_input()
// {
//     double in1, in2;

//     if (scanf(" [ %lf , %lf ] ", &in1, &in2) != 2)
//     {
//         printf("Nespravny vstup.\n");
//         return 0;
//     }

//     return 1;
// }


int main()
{
    //variables to save user input
    double xa = 0, xb = 0, xc = 0;
    double ya = 0, yb = 0, yc = 0;

    //load user input
    printf("Bod A:\n");
    if (scanf(" [ %lf , %lf ] ", &xa, &ya) != 2)
    {
        printf("Nespravny vstup.\n");
        return 0;
    }

    printf("Bod B:\n");
    if (scanf(" [ %lf , %lf ] ", &xb, &yb) != 2)
    {
        printf("Nespravny vstup.\n");
        return 0;
    }

    printf("Bod C:\n");
    if (scanf(" [ %lf , %lf ] ", &xc, &yc) != 2)
    {
        printf("Nespravny vstup.\n");
        return 0;
    }


    //Work with user input
    

    return 0;
}