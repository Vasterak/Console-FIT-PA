#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

void swap_values(int ** a, int ** b)
{
    printf("------------------------\n");
    printf("A>  address: %p, value: %p\n", &a, a);
    printf("A>                           value: %p, dereferenced1: %p\n", a, *a);
    printf("A0>                                                 dereferenced1: %p, dereferenced2: %d\n", *a, **a);
    printf("A1>                                                 dereferenced1: %p, dereferenced2: %d\n", (*a)+1, *((*a)+1));



    //printf("Y> address: %p, value: %d\n", *b, *b[1]);
    int * c;
    printf("------------------------\n");
    printf("A>  address: %p, value: %p\n", &a, a);
    printf("B>  address: %p, value: %p\n", &b, b);
    printf("C>  address: %p, value: %p\n", &c, c);
    c = *a;
    *a = *b;
    *b = c;
    printf("after swap\n");
    printf("A>  address: %p, value: %p\n", &a, a);
    printf("B>  address: %p, value: %p\n", &b, b);
    printf("C>  address: %p, value: %p\n", &c, c);

        printf("------------------------\n");
    printf("A>  address: %p, value: %p\n", &a, a);
    printf("A>                           value: %p, dereferenced1: %p\n", a, *a);
    printf("A0>                                                 dereferenced1: %p, dereferenced2: %d\n", *a, **a);
    printf("A1>                                                 dereferenced1: %p, dereferenced2: %d\n", (*a)+1, *((*a)+1));
}

int main()
{
    int * a = (int *) malloc (sizeof(*a) * 5);
    int * b = (int *) malloc (sizeof(*b) * 10);

    a[0] = 45;
    a[1] = 92;

    b[0] = 999;
    b[1] = 50;

    printf("A>  address: %p, value: %p\n", &a, a);
    printf("A0>                          value: %p, dereferecned: %d\n", a, *a);
    printf("A1>                          value: %p, dereferecned: %d\n", a+1, *(a+1));


    printf("%d, %d, %d, %d\n", a[0], a[1], b[0], b[1]);
    printf("SWAP TIME\n");
    swap_values(&a, &b);
    printf("%d, %d, %d, %d\n", a[0], a[1], b[0], b[1]);
    free(a);
    free(b);

    return 0;
}