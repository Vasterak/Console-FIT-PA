#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

//CONSTANTS
#define ARRAYLIMIT 100000

//VARIABLES
// structure that holds all the carriers
typedef struct
{
    unsigned from;
    unsigned to;
    unsigned packages;
    unsigned price;
} CARRIERS;

//global variables, not great not terrible
CARRIERS listCarriers[ARRAYLIMIT];
unsigned lengthCarriers = 0;
unsigned long long totalPrice = 0;
unsigned long long lastMaxDay = 0;

//FUNCTIONS
//get next character in stdin
char peak_char()
{
    char c = getchar();
    ungetc(c, stdin);
    return c;
}

//remove all white spaces
void clear_whitespace()
{
    while (isspace(peak_char()))
        getchar();
}

//skip whitespace and check what next character is
bool read_next_char(char input)
{
    clear_whitespace();
    if (input == peak_char())
    {
        //if it is the desired char then remove it from stdin
        getchar();
        return true;
    }
    else
        return false;
}

//qsort compare function
int cmp_func(const void *a, const void *b)
{
    // unsigned num1 = *(const unsigned *)a;
    // unsigned num2 = *(const unsigned *)b;
    // return (num1 > num2) - (num2 > num1);
    return ((CARRIERS*)a)->from - ((CARRIERS*)b)->from;
}

bool is_valid_carrier(int from, int to, int packages, int price)
{
    if (from < 0 || to <= 0 || from > to)
        return false;

    if (packages <= 0 || price <= 0)
        return false;

    return true;
}

bool is_valid_cargo(long long startDay, long long cargo)
{
    if (startDay < 0 || cargo <= 0)
        return false;
    else
        return true;
}

void error_msg()
{
    printf("Nespravny vstup.\n");
}

void add_to_list(unsigned from, unsigned to, unsigned packages, unsigned price)
{
    listCarriers[lengthCarriers - 1].from = from;
    listCarriers[lengthCarriers - 1].to = to;
    listCarriers[lengthCarriers - 1].packages = packages;
    listCarriers[lengthCarriers - 1].price = price;

    //save max day value
    if (to > lastMaxDay)
        lastMaxDay = to;
}

bool carriers_input()
{
    // input variables
    int from = 0, to = 0, packages = 0, price = 0;

    //scanf variable
    int scanfOut = 0;
    scanfOut = scanf(" { [ %d - %d , %d , %d", &from, &to, &packages, &price);

    if (!read_next_char(']'))
        return false;

    if (scanfOut != 4 || !is_valid_carrier(from, to, packages, price))
        return false;

    lengthCarriers++;

    //save values
    add_to_list(from, to, packages, price);

    //check if array continues, aka more carriers are in input
    if (read_next_char(','))
    {
        //read carriers until end or 100 000
        while (true)
        {
            if (scanf(" [ %d - %d , %d , %d", &from, &to, &packages, &price) != 4)
                return false;

            if (!read_next_char(']'))
                return false;

            if (!is_valid_carrier(from, to, packages, price))
                return false;

            //check length
            lengthCarriers++;
            if (lengthCarriers > ARRAYLIMIT)
                return false;

            //save values
            add_to_list(from, to, packages, price);

            //check if this carrier is the last in input list then break from loop
            if (!read_next_char(','))
                break;
        }
    }

    if (read_next_char('}'))
        return true;
    else
        return false;
}

bool calculate_cargo(unsigned long long day, unsigned long long _cargo, unsigned long long *_totalCost, unsigned long long *_endDay)
{
    //setup variables
    long long cargo = _cargo;
    unsigned long long cost = 0;

    //if selected day is lower then any availabe carrier then there is not a valid reason to no do this.
    if (day <= listCarriers[0].from)
        day = listCarriers[0].from;

    while (1)
    {
        // pokud je aktualni den vetsi, tak musime zjistit jak moc je vetsi. IDK jsem hloupej jak to najit, takze mi nezbyva nez to projet cele to pole
        // for loop
        for (size_t i = 0; i < lengthCarriers; i++)
        {
            // small help, but the longer in loop the more useless it is
            if (day < listCarriers[i].from)
                continue;

            // pokud den se nachazi v intervalu
            if (day >= listCarriers[i].from && day <= listCarriers[i].to)
            {
                cost += listCarriers[i].price;
                cargo -= listCarriers[i].packages;
            }
        }

        if (cargo <= 0)
            break;

        day++;

        if (day > lastMaxDay)
            return false;
    }

    *_endDay = day;
    *_totalCost = cost;

    return true;
}

int main()
{
    printf("Moznosti dopravy:\n");

    //load all carriers
    if (!carriers_input())
    {
        error_msg();
        return 0;
    }

    // test print
    // printf("nacteno bylo vstupu %d\n", lengthCarriers);
    // printf("vypis toho co bylo nacteno.\n");
    // for (size_t i = 0; i < lengthCarriers; i++)
    // {
    //     printf(" %d - %d, %d, %d\n", listCarriers[i].from, listCarriers[i].to, listCarriers[i].packages, listCarriers[i].price);
    // }

    //sort the list using qsort from starting date
    qsort(listCarriers, lengthCarriers, sizeof(CARRIERS), cmp_func);

    //test print
    // printf("bylo serazeno.\n");
    // for (size_t i = 0; i < lengthCarriers; i++)
    // {
    //     printf(" %d - %d, %d, %d\n", listCarriers[i].from, listCarriers[i].to, listCarriers[i].packages, listCarriers[i].price);
    // }

    printf("Naklad:\n");


    //variables
    long long startDay = 0, cargo = 0;
    unsigned long long totalCost = 0, endDay;
    int scanfErrorCodes = 0;
 
    //loop until EOF
    while (true)
    {
        //read user input
        scanfErrorCodes = scanf(" %lld %lld", &startDay, &cargo);
        if (scanfErrorCodes != 2)
        {
            if (scanfErrorCodes == EOF)
                break;
            else
            {
                error_msg();
                return 0;
            }

        }

        //check if there is left some garbage in stdin

        //check if input numbers are in valid range
        if (!is_valid_cargo(startDay, cargo))
        {
            error_msg();
            return 0;
        }

        //calculate cargo
        if (!calculate_cargo(startDay, cargo, &totalCost, &endDay))
            printf("Prilis velky naklad, nelze odvezt.\n");
        else
            printf("Konec: %lld, cena: %lld\n", endDay, totalCost);

        totalCost = 0;
        endDay = 0;
    }
    

    return 0;
}