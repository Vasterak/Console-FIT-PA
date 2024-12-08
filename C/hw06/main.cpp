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
    //MAIN VARIABLES
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

    //READ INPUT
    char *inputLine = NULL;
    size_t allocatedLength = 10;
    size_t readLength = 0;

    printf("Write input\n");
    printf("Alloc length before input read: %ld\n", allocatedLength);
    readLength = getline(&inputLine, &allocatedLength, stdin);
    printf("Alloc length after input read: %ld\n", allocatedLength);
    printf("total length after input read: %ld\n", readLength);

    

    printf("input text: %s", inputLine);


    //free 2d array
    for (size_t i = 0; i < puzzleAllocWidth; i++)
        free(puzzle[i]);
    free(puzzle);

    return 0;
}