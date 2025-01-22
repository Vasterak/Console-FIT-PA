#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

char peek_char()
{
    char a = getchar();
    if (a != EOF)
        ungetc(a, stdin);
    return a;
}

void clear_whitespace()
{
    char a;
    while ((a = peek_char()) && (a != EOF) && (isspace(a)) && (a != '\n'))
        getchar();
}

bool is_clean_buffer()
{
    //remove existing white spaces
    clear_whitespace();
    //check characters after a white space
    if (!isspace(peek_char()))
    {
        printf("you have input a non valid character after your number!\n");
        return false;
    }
    return true;
}

int main()
{
    printf("Input single integer:\n");

    int scanfout, numberInput;
    scanfout = scanf(" %d", &numberInput);
    if (scanfout != 1)
    {
        printf("Error1\n");
        return 0;
    }

    //check content after stdin
    if (!is_clean_buffer())
    {
        printf("Error2\n");
        return 0;
    }

    printf("Input number was: %d\n", numberInput);
}