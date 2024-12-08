#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

// Macro "function" to toggle debug messages
// https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) do { fprintf(stdout, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__); } while (0)
#else
    #define DEBUG_PRINT(...) do {} while (0)
#endif

// CONSTANTS
const size_t DEFAULTALLOC = 4;

int main()
{
    //VARIABLES
    char **puzzle = NULL;

    size_t puzzleWidth = 0;
    size_t puzzleHeight = 0;

    size_t puzzleAllocWidth = DEFAULTALLOC + 2;
    size_t puzzleAllocHeight = DEFAULTALLOC * 3;
    
    //Default allocations
    puzzle = (char**) malloc(sizeof(char *) * puzzleAllocWidth);
    if (puzzle == NULL)
        return 1;
    
    for (size_t i = 0; i < puzzleAllocWidth; i++)
    {
        puzzle[i] = (char *) malloc(sizeof(char) * puzzleAllocHeight);
        if (puzzle[i] == NULL)
            return 1;
    }
    
    //Write values to it
    for (size_t i = 0; i < puzzleAllocWidth; i++)
    {
        puzzle[i][0] = 'A';
        int tmp = 48;
        for (size_t j = 1; j < puzzleAllocHeight; j++)
        {
            tmp+=1;
            puzzle[i][j] = 'A';
        }
    }

    //print it
    for (size_t i = 0; i < puzzleAllocHeight; i++)
    {
        for (size_t j = 0; j < puzzleAllocWidth; j++)
        {
            printf("%c",puzzle[j][i] );
        }
        printf("\n");
    }
        printf("\n");

    //free 2d array
    for (size_t i = 0; i < puzzleAllocWidth; i++)
        free(puzzle[i]);
    free(puzzle);

    return 0;
}