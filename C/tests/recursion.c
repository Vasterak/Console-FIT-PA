#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

int tiles(const int brickSize[], int brickSizeLength, int wallLength)
{
    size_t totalVariants = 0;

    //wall has been briced corretly
    if (wallLength == 0)
        return 1;

    //cannot lay brics, the brics is bigger then the remaining wall length
    if (wallLength < 0)
        return 0;

    //repeat for all brics variants
    for (int i = 0; i < brickSizeLength; i++)
    {
        totalVariants += tiles(brickSize, brickSizeLength, wallLength - brickSize[i]);
    }
    return totalVariants;
}

int tiles_print_impl(const int brickSize[], int brickSizeLength, int wallLength, char * buffer)
{
    //wall has been briced corretly
    if (wallLength == 0)
    {
        //print the valid solution
        printf("Varianty dlazdicek: %s\n", buffer);
        return 1;
    }

    //cannot lay brics, the brics is bigger then the remaining wall length
    if (wallLength < 0)
        return 0;

    size_t totalVariants = 0;
    char * tmp = buffer + strlen(buffer); //get to the end of the "string" to start writing at that position

    //repeat for all brics variants
    for (int i = 0; i < brickSizeLength; i++)
    {
        sprintf(tmp, " %d", brickSize[i]); // add the brick width.
        totalVariants += tiles_print_impl(brickSize, brickSizeLength, wallLength - brickSize[i], buffer);
    }
    return totalVariants;
}

int tiles_fast_imp(const int brickSize[], int brickSizeLength, int wallLength, int *table)
{
    //wall has been briced corretly
    if (wallLength == 0)
        return 1;

    //cannot lay brics, the brics is bigger then the remaining wall length
    if (wallLength < 0)
        return 0;

    if (table[wallLength] == -1)
    {
        size_t totalVariants = 0;
        //repeat for all brics variants
        for (int i = 0; i < brickSizeLength; i++)
        {
            totalVariants += tiles_fast_imp(brickSize, brickSizeLength, wallLength - brickSize[i], table);
        }
        table[wallLength] = totalVariants;
    }
    return table[wallLength];
}

int tiles_fast(const int brickSize[], int brickSizeLength, int wallLength)
{
    int *table = (int *) malloc ( (wallLength + 1) * sizeof(int));
    for (int i = 0; i < wallLength+1; i++)
        table[i] = -1;

    int result = tiles_fast_imp(brickSize, brickSizeLength, wallLength, table);
    free(table);
    return result;
}
int tiles_print(const int brickSize[], int brickSizeLength, int wallLength)
{
    char buffer[100] = { '\0' };
    return tiles_print_impl(brickSize, brickSizeLength, wallLength, buffer);
}


int main()
{
    const int brickSizes[] = {5,3,1};
    const int brickSizesLength = 3;
    const int wallLength = 40;

    printf("Number of variants to build the wall: %d\n", tiles(brickSizes, brickSizesLength, wallLength));
    //printf("Print  of variants to build the wall: %d\n", tiles_print(brickSizes, brickSizesLength, wallLength));
    printf("Fast      variants to build the wall: %d\n", tiles_fast(brickSizes, brickSizesLength, wallLength));



    return 0;
}