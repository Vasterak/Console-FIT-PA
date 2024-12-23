#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

typedef struct node
{
    int value;
    struct node * next;
} NODE;

void print_linked_list( NODE *head)
{
    NODE *temp = head;

    while (temp != NULL)
    {
        printf("value: %d\n", temp->value);
        temp = temp->next;
    }
}

bool is_valid_input(const char *phoneNumber)
{
    size_t index = 0;
    while (phoneNumber[index] != '\0')
    {
        printf("kontroluji: %c\n", phoneNumber[index]);
        if (phoneNumber[index] < '0' || phoneNumber[index] > '9')
            return false;
        index++;
    }
    return true;
}

int main()
{
    //test setup
    // NODE n1, n2, n3, n4;
    // NODE *head;

    // n1.value = 1;
    // n2.value = 2;
    // n3.value = 3;
    // n4.value = 4;

    // //link together

    // head = &n1;
    // n1.next = &n2;
    // n2.next = &n3;
    // n3.next = &n4;
    // n4.next = NULL;

    // print_linked_list(head);

    char cislo[] = "420668114789";

    if (is_valid_input(cislo))
    {
        printf("Cislo je spravne.\n");
    }
    else
        printf("Cislo je spatne.\n");

    return 0;
}