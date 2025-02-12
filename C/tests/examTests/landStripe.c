#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

/* DEFINITION OF THIS TEST
Úkolem je realizovat program, který bude optimalizovat výkupní cenu za pozemky pro liniovou komunikaci v cenové mapé pozemkủ. Predpokládáme, ze známe cenovou mapu pozemky pro néjaké územi. Mapa má tvar obdélníku a je rozdélena na W x H elementárních parcel čtvercového tvaru v pravoúhlém rastru. Pro kazdou parcelu známe její cenu (celé císlo, musí být vetsí nez 0). V daném území má být vybudována cesta severo-jizním smérem (smérem shora doli). Pro cestu je potreba vykoupit pás pozemki pres celou vysku mapy. Pro vykup pozemku máme danou cenu P, za kterou by pozemky mély byt vykoupené. Cílem je najít takový pás pozemku, ktery bude zadané cené co nejpodobnejsi. Je jedno, jak bude pás široky a je jedno, kde přesně bude umisteny (na kterych souradnicich x). Obecné muze existovat více mozností koupé za cenu stejné blízkou P. V takovém prípadé program vypise vsechny moznosti. Obrázek zachycuje mapu velikosti 4×5 a pozadovanou cenu P-15:

Vstupem programu je cenová mapa a zadání výkupních cen. Cenová mapa je zadaná jako první. Nejprve jsou uvedena dvé celá kladná cisla - Sírka a výska cenové mapy. Pak následuje W x H celých cisel s cenami jednotlivých elementárních parcel. Ceny jsou zadané po rádcich. V ukázkovych bézich je vstup zformátován tak, aby byl snáze citelný. Pri zpracování vstupu nekontrolujte, zda jsou ceny parcel zformátované do obdélníkového tvaru se správným poctem rádek a sloupcủ, pouze nactéte a zkontrolujte W x H celých cisel. Po zadání cenové mapy následují pokyny k výpoctu:

 ? P - výpocet pozice vykupovaného pásu pozemky. Zadané je celé kladné císlo P, tj. cena, za kterou by pás pozemky měl být ideálné vykoupen. Výstupem jsou nalezené pásy, které se cenou co nejvíce blízi zadání,
[x, y] =a - zmena ceny v pozemkové mapé. Nová cena pozemku na pozici (x,y) je a. Není ocekávaný zádný výstup.
Zadáváni koncí dosazenim konce vstupu (EOF).

Vystupem programu jsou nalezené pozice vykupovaných pású pozemkú. Program zobrazí pocátecní a koncový sloupec vykupovaného pásu pozemkú a jeho celkovou cenu. Sloupce indexujeme od O. Vúpis moznosti zacíná znakem *, poradi nalezených pási ve výpisu není urceno (muze být libovolné).

Pokud vstup není platný, program tuto situaci detekuje, vypíse chybové hlásení a ukoncí se. Formát chybového hlásení je opët uveden v ukázkách níze. Za chybu je povazováno:

sirka/výska cenové mapy není celé kladné císlo (musí být vétsí nez 0),
cena elementární parcely není celé kladné císlo (cena musí být vetsi nez 0),
nerozpoznany prikaz (musí být bud' ? nebo […) -….),
pro hledání pozice vykupovaného pásu pozemkú: cena není kladné celé císlo (musí být vétsí nez. 0),
pro zménu ceny pozemku: pozice pozemku je mimo rozsah mapy, nová cena pozemku není celé kladné císlo.
Pokud program detekuje chybu, prestane se dotazovat na dalsi vstupni hodnoty, vypise chybove hlaseni a ukonci se. Chybu je tedy potreba detekovat okamzitě po nactení hodnoty (neodkládejte kontrolu vstupnich údajú az za nacteni celého vstupu). Chybové hlásení vypisujte na standardní výstup (nevypisujte jej na standardni chybový vystup).

Vás program bude spoustén v omezeném testovacím prostiedí. Je omezen dobou behu (limit je vidét v logu refereneního resení) a dále se kontroluje i velikost pouzité paméti (k dispozici je dostatek paméti pro ulození cenové mapy a dalsích vypoctených hodnot; pamētové pozadavky vsak nesmí nesmysiné prekracovat rozumné meze). Program ale musí pamét pro cenovou mapu alokovat dynamicky. Zadaný problém (jedno hledání pozice pásu) lze resit algoritmy s rúznou efektivitou:

Naivni resení má slozitost 0 (W2×H). Takové resení neprojde zádným z testú rychlosti.
Vylepsené resení má slozitost 0 (w2). Takové resení projde prvním testem rychlosti.
Optimální resení má slozitost O (w). Takové resení projde obema testy rychlosti.
(> vstup, < výstup)
*/

/*
3 8 2 1
5 1 3 2
2 8 1 1
1 7 2 2
3 4 1 1
*/

/* EXAMPLE FOR RUNNING THIS CODE
Ukazka Chodu
< Mapa:
> 4 5
> 3 8 2 1
> 5 1 3 2
> 2 8 1 1
> 1 7 2 2
> 3 4 1 1
< Problemy:
> ? 10
< Moznosti: 1
< * 2 -> 2: 9
> [ 0 , 0 ] = 5
> [2,2]=4
> ? abcdef
< Nespravny vstup.

< Mapa:
> 3 2
> 1 2 1
> 2 1 2
< Problem: 
> ? 3
< Moznosti: 3
< * 0 -> 0: 3
< * 1 -> 1: 3
< * 2 -> 2: 3
> ? 6
< Moznosti: 2
< * 0 -> 1: 6
< * 1 -> 2: 6
> ? 9
< Moznosti: 1
< * 0 -> 2: 9 
*/

typedef struct
{
    size_t height;
    size_t width;
    int **plots;
} Plots2D;

typedef struct
{
    size_t start;
    size_t end;
    int price;
} SavedPlots;


void memory_alloc(Plots2D *arrays)
{
    arrays->plots = (int**) malloc(sizeof(int*) * arrays->height);

    for (size_t i = 0; i < arrays->height; i++)
        arrays->plots[i] = (int*) malloc(sizeof(int) * arrays->width);
}

void free_array(Plots2D *arrays)
{
    for (size_t i = 0; i < arrays->height; i++)
        free(arrays->plots[i]);
    free(arrays->plots);
}

void array_print(const Plots2D *arrays)
{
    for (size_t i = 0; i < arrays->height; i++)
    {
        for (size_t j = 0; j < arrays->width; j++)
        {
            printf("%d ", arrays->plots[i][j]);
        }
        printf("\n");
    }
}

bool user_input(Plots2D *arrays)
{
    //set the dimentions from user input
    int width = 0, height = 0;
    int scanfout = scanf("%d %d", &width, &height);
    if (scanfout != 2)
        return false;
    if (width <= 0 || height <= 0)
        return false;

    arrays->height = height;
    arrays->width = width;

    //allocate memory
    memory_alloc(arrays);

    //set the plot values from user input
    int inputValue = 0;
    for (size_t i = 0; i < arrays->height; i++)
    {
        for (size_t j = 0; j < arrays->width; j++)
        {
            if(scanf(" %d", &inputValue) != 1)
            {
                printf("Error while reading 2d plot values.\n");
                return false;
            }
            if (inputValue <= 0)
                return false;
            //save the value to the array
            arrays->plots[i][j] = inputValue;
        }
    }
    return true;
}

void get_best_price(const Plots2D *arrays, SavedPlots *saved, int desiredPrice)
{
    size_t savedIndex = 0;
    int currentBestDiviation = INT_MAX;
    int **array = arrays->plots;

    int columnSumary = 0;

    //iterate over the starting column
    for (size_t i = 0; i < arrays->width; i++)
    {
        //looping throught each column, so single run
        for (size_t j = i; j < arrays->width; j++)
        {
            //sum-up the whole single column
            for (size_t k = 0; k < arrays->height; k++)
            {
                columnSumary += array[k][j];
            }

            //comparate it to the required price, is even better, so is lower. then delete all previews best and start over
            if (abs(columnSumary - desiredPrice) < currentBestDiviation)
            {
                //set the new best to compare variable
                currentBestDiviation = abs(columnSumary - desiredPrice);

                //Clear the array
                savedIndex = 0;
                saved[savedIndex].start = i;
                saved[savedIndex].end = j;
                saved[savedIndex].price = columnSumary;

            }
            //if the diviation is the same, then save it as well next to it.
            else if (abs(columnSumary - desiredPrice) == currentBestDiviation)
            {
                savedIndex++;
                saved[savedIndex].start = i;
                saved[savedIndex].end = j;
                saved[savedIndex].price = columnSumary;
            }
            /*          IDK but this can help? as if "else" is called then the columnSumary is rising and getting away from the desiredPrice, so there is no point in continuing?
            else
                break;
            */
            
        }
        //reset the culumn summary per run.
        columnSumary = 0;
    }

    //print it
    printf("Moznosti: %ld\n", savedIndex+1);
    for (size_t i = 0; i < savedIndex+1; i++)
    {
        printf(" %ld -> %ld: %d\n", saved[i].start, saved[i].end, saved[i].price);
    }
    printf("--------ENDE-----------\n");
}

int main()
{
    Plots2D plots;
    SavedPlots *saved;
    printf("Mapa:\n");

    if (!user_input(&plots))
    {
        printf("There was an error while reading user input.\n");
        return 0;
    }

    //create a location to save plot values, max values is for each column which is same as it's width.
    saved = (SavedPlots*) calloc(plots.width, sizeof(SavedPlots));

    printf("-------------\n");

    array_print(&plots);

    printf("Problemy:\n");

    //main loop
    while (true)
    {
        char inputChar;
        int desiredPrice = 0;
        int scanfout = scanf(" %c", &inputChar);
        if (scanfout == EOF)
            break;

        int coordHeight = 0, coordWidth = 0, newPrice = 0;
        
        switch (inputChar)
        {
        //ask for the best plot to buy!
        case '?':
            scanfout = scanf(" %d", &desiredPrice);
            if (scanfout != 1)
            {
                printf("bad input.\n");
                return 0;
            }
            get_best_price(&plots, saved, desiredPrice);

            break;

        //change the price of the specific plot!
        case '[':

            scanfout = scanf(" %d , %d ] = %d", &coordWidth, &coordHeight, &newPrice);
            if (scanfout != 3 || coordWidth <= 0 || coordHeight <= 0 || newPrice <= 0 || coordWidth >= (int)plots.width || coordHeight >= (int)plots.height)
            {
                printf("spatny vstup nove souradnice voe!");
                return 0;
            }
            //set the new price
            plots.plots[coordHeight][coordWidth] = newPrice;

            //DEBUG PRINT
            printf("--------------------\n");
            array_print(&plots);

            break;
        
        default:
            break;
        }
    }
    

    free_array(&plots);

    return 0;
}