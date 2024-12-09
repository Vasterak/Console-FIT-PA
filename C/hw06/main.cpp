#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

#define DEBUG

// Macro "function" to toggle debug messages
// https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) do { fprintf(stdout, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__); } while (0)
#else
    #define DEBUG_PRINT(...) do {} while (0)
#endif

//CONSTANTS
const size_t DEFAULT_HEIGHT = 2;
const size_t MAX_LINE_LENGTH = 60;

//STRUCTS
typedef struct puzzleSpec
{
    size_t height = 0;
    size_t width = 0;
    size_t allocWidth = 0; //will be set by first input line, wont be changed
    size_t allocHeight = DEFAULT_HEIGHT;
} PuzzleSpec;


//FUNCTIONS
char peak_char()
{
    char a = getchar();
    ungetc(a, stdin);
    return a;
}

void clear_whitespace()
{
    while (isspace(peak_char()))
        getchar();
}

void error_msg()
{
    printf("Nespravny vstup.\n");
}

bool check_string(char* text, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        if (text[i] != '.')
        {
            if (text[i] < 'a' || text[i] > 'y')
            {
                if (text[i] == '\n')
                {
                    //check if input is only newline at the begining meaning no valid input
                    if (length == 1)
                    {
                        DEBUG_PRINT("vstup byl moc kratky, zadal jsi pouze enter: %s\n", text);
                        return false;
                    }
                    
                    //if string is longer then 1 char then just remove the \n because this is valid input
                    text[i] = '\0';
                }
                //input is not a '.' or a-z or \n
                else
                    return false;
            }
        }
    }
    return true;
}

void print_secret(char *** puzzle, PuzzleSpec *puzzleData, bool *** checked)
{
    size_t currentLength = 0;

    for (size_t i = 0; i < puzzleData->height; i++)
    {
        for (size_t j = 0; j < puzzleData->width; j++)
        {
            if ((*checked)[i][j] == false && (*puzzle)[i][j] != '.')
            {
                if (currentLength == MAX_LINE_LENGTH)
                {
                    currentLength = 0;
                    printf("\n");
                }

                printf("%c", (*puzzle)[i][j]);
                currentLength++;
            }

        }
        
    }
    
}

void find_word(char *** puzzle, PuzzleSpec *puzzleData)
{

}

void cross_word(char *** puzzle, PuzzleSpec *puzzleData, bool *** checked)
{

}

bool read_puzzle(char *** puzzle, PuzzleSpec *puzzleData)
{
    //GETLINE variables
    char *inputLine = NULL;
    size_t allocLength = 10;
    ssize_t readLength = 0;

    //initial read, reading the first line is different from reading the rest of the lines, so it is not inside a loop

    ssize_t setWidth = getline(&inputLine, &allocLength, stdin);      //set initial width from now on, it is required to has this and only this width.
    puzzleData->allocWidth = setWidth + 1;                            // added +1 because valid input is also just 'a' and will be entered with 'char' + "ctrl + d" so no added \n to become as second char.

    DEBUG_PRINT("setwidth: %ld\n", setWidth);

    if (setWidth == -1)
    {
        DEBUG_PRINT("%s\n", "spatny prvni vstup krizovnky");
        return false;
    }

    //check for valid data inside input string
    if (!check_string(inputLine, setWidth))
        return false;

    //alloc first line of values to puzzle 2d array
    (*puzzle)[puzzleData->height] = (char*) malloc(sizeof(char) * puzzleData->allocWidth);  
    strncpy((*puzzle)[0], inputLine, puzzleData->allocWidth);
    puzzleData->height++;

    //Reading rest of the lines
    while (true)
    {
        readLength = getline(&inputLine, &allocLength, stdin);
        if (readLength == -1)
        {
            DEBUG_PRINT("%s\n", "Error while reading");
            free(inputLine);
            return false;
        }

        //detect the end of input
        if (readLength == 1)
        {
            if (inputLine[0] == '\n')
                break;
        }

        if (readLength != setWidth)
        {
            DEBUG_PRINT("%s\n", "Neplatna sirka");
            free(inputLine);
            return false;
        }


        //check for valid data inside input string
        if (!check_string(inputLine, setWidth))
        {
            free(inputLine);
            return false;
        }

        //check if realloc is needed for height
        if (puzzleData->height == puzzleData->allocHeight)
        {
            DEBUG_PRINT("%s\n","Realloc");
            DEBUG_PRINT("Adresa pred: %p\n", (void *) *puzzle);

            puzzleData->allocHeight *= 2;
            *puzzle = (char**) realloc(*puzzle, puzzleData->allocHeight * sizeof(char*));
            if (*puzzle == NULL)
                return false;

            DEBUG_PRINT("Adresa po: %p\n", (void *) *puzzle);
        }

        (*puzzle)[puzzleData->height] = (char*) malloc(sizeof(char) * puzzleData->allocWidth);  
        strncpy((*puzzle)[puzzleData->height], inputLine, puzzleData->allocWidth);
        puzzleData->height++;

        DEBUG_PRINT("%ld\n", puzzleData->height);
    }

    puzzleData->width = puzzleData->allocWidth - 2;
    free(inputLine);
    return true;
}

int main()
{
    //define puzzle 2D array
    char **puzzle = NULL;
    PuzzleSpec puzzleData;
    
    //Allocate first X height of the puzzle
    puzzle = (char**) malloc(sizeof(char *) * puzzleData.allocHeight);
    if (puzzle == NULL)
        return 1;

    printf("Osmismerka:\n");

    //read input 2d array
    if (!read_puzzle(&puzzle, &puzzleData))
    {
        error_msg();
        //freeup existing memory!
        return 1;
    }

    DEBUG_PRINT("%s\n", "Prechod na dotazy");
    DEBUG_PRINT("Sirka pole:%ld\n", puzzleData.width);
    DEBUG_PRINT("Vyska pole:%ld\n", puzzleData.height);

    //create a bool 2d array for checking already used letters
    bool **checked = NULL;
    checked = (bool **) calloc(puzzleData.height, sizeof(bool*));
    for (size_t i = 0; i < puzzleData.height; i++)
        checked[i] = (bool *) calloc(puzzleData.width, sizeof(bool));

    //Input querries
    int scanfCodes = 0;
    char operationChar = '\0';
    while (true)
    {
        scanfCodes = scanf(" %c", &operationChar);
        if (scanfCodes != 1)
        {
            if (scanfCodes == EOF)
                break;
            else
            {
                error_msg();
                //freeup existing memory!
                return 1;
            }
        }

        switch (operationChar)
        {
        case '-':
            clear_whitespace();
            cross_word(&puzzle, &puzzleData, &checked);
            break;
            
        case '#':
            clear_whitespace();
            find_word(&puzzle, &puzzleData);
            break;

        case '?':
            printf("Tajenka:\n");
            print_secret(&puzzle, &puzzleData, &checked);
            break;
        
        default:
            error_msg();
            return 1;
        }
    }

    //free 2d array
    for (size_t i = 0; i < puzzleData.height; i++)
        free(puzzle[i]);
    free(puzzle);

    return 0;
}