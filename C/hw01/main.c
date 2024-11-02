#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>

const double EPSILON = 0.0001;

char peekchar()
{
    char c = getchar();
    ungetc(c, stdin);
    return c;
}

void skip_whitespace()
{
    char a;
    while (true)
    {
        a = peekchar();

        if (isspace(a))
        {
            if (a == '\n')
                break;
            getchar();
        }
        else
            break;
    }

}

bool is_double_eq(double a, double b)
{
    if (fabs( a - b) < EPSILON)
        return true;
    else
        return false;
}

// get slope
bool check_straight(double x1, double x2, double x3, double y1, double y2, double y3 )
{
    double a,b;
    a = (y2 - y1) / (x2 - x1);
    b = (y3 - y2) / (x3 - x2);

    if (fabs(a - b) < EPSILON)
        return 1;
    else
        return 0;
}

bool is_perpendicular(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
    double temp1, temp2;

    // When only line 1 has infinite slope
    if (fabs(x2 - x1) < EPSILON)
    {

        temp2 = (y4 - y3) / (x4 - x3);

        if (fabs(temp2) < EPSILON)
            return true;
        else
            return false;
    }

    // Only line 2 has infinite slope
    else if (fabs(x4 - x3) < EPSILON)
    {

        temp1 = (y2 - y1) / (x2 - x1);

        if (fabs(temp1) < EPSILON)
            return true;
        else
            return false;
    }

    else
    {
        // Find slopes of the lines
        temp1 = (y2 - y1) / (x2 - x1);
        temp2 = (y4 - y3) / (x4 - x3);

        // Check if their product is -1
        //printf("temp1: %lf; temp2: %lf\n", temp1, temp2);

        //        if (temp1 * temp2 -1)
        if ((temp1 * temp2 + 1) < EPSILON && (temp1 * temp2 + 1) > (EPSILON * -1))
            return true;
        else
            return false;
    }
}

//calculate distance between 2 points
double get_distance(double x1, double y1, double x2, double y2)
{
    double cal = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return cal;
}

// return what type of parallelogram it is
const char * get_parallelogram_type(double diagonal1, double diagonal2, double parallel1, double parallel2, double x1, double y1, double x2, double y2, double a1, double b1, double a2, double b2)
{
    //if diagonals are equal
    //printf("diagonal1: %lf; diagonal2: %lf\n", diagonal1, diagonal2);

    if (fabs(diagonal1 - diagonal2) < EPSILON)
    {
        //if paralels are also equal then it is square == ctverec otherwise it is rectangle == obdelnik
        if (fabs(parallel1 - parallel2) < EPSILON)
            return "ctverec";
        else
            return "obdelnik";
    }
    else
    {
        if (is_perpendicular(x1, y1, x2, y2, a1, b1, a2, b2))
            return "kosoctverec";
        else
            return "rovnobeznik";
    }
}

int main()
{
    //variables to save user input
    double x1 = 0, x2 = 0, x3 = 0;
    double y1 = 0, y2 = 0, y3 = 0;

    //load user input
    printf("Bod A:\n");
    if (scanf(" [ %lf , %lf ]", &x1, &y1) != 2)
    {
        printf("Nespravny vstup.\n");
        return 0;
    }
    skip_whitespace();
    if (peekchar() == ']')
    {
        printf("Nespravny vstup.\n");
        return 0;
    }

    printf("Bod B:\n");
    if (scanf(" [ %lf , %lf ]", &x2, &y2) != 2)
    {
        printf("Nespravny vstup.\n");
        return 0;
    }
    skip_whitespace();
    if (peekchar() == ']')
    {
        printf("Nespravny vstup.\n");
        return 0;
    }
    // if (peekchar() != '[')
    // {
    //     printf("Nespravny vstup4.\n");
    //     return 0;
    // }

    printf("Bod C:\n");
    if (scanf(" [ %lf , %lf ]", &x3, &y3) != 2)
    {
        printf("Nespravny vstup.\n");
        return 0;
    }
    if (!isspace(peekchar()))
    {
        if (peekchar() != '[')
        {
            printf("Nespravny vstup.\n");
            return 0;
        }
    }

    //Work with user input

    //all 3 points have same cords
    if ((is_double_eq(x1, x2) && is_double_eq(x2, x3)) || (is_double_eq(y1, y2) && is_double_eq(y2, y3)))
    {
        printf("Rovnobezniky nelze sestrojit.\n");
        return 0;
    }

    //only two points are same
    if ((is_double_eq(x1, x2) && is_double_eq(y1, y2)) || (is_double_eq(x1, x3) && is_double_eq(y1, y3)) || (is_double_eq(x2, x3) && is_double_eq(y2, y3)))
    {
        printf("Rovnobezniky nelze sestrojit.\n");
        return 0;
    }

    //Check if points are not located on the same line.
    if (check_straight(x1, x2, x3, y1, y2, y3) == true)
    {
        printf("Rovnobezniky nelze sestrojit.\n");
        return 0;
    }

    //Calculate the last point
    double x4 = 0;
    double y4 = 0;
    
    //------Variant for ABA'C------
    x4 = x2 + x3 - x1;
    y4 = y2 + y3 - y1;

    printf("A': [%lf,%lf], %s\n", x4, y4, get_parallelogram_type(get_distance(x1, y1, x4, y4), get_distance(x2, y2, x3, y3),
                                                                 get_distance(x1, y1, x3, y3), get_distance(x1, y1, x2, y2),
                                                                 x1, y1, x4, y4, x2, y2, x3, y3));

    //------Variant for ABCB'------
    x4 = x1 + x3 - x2;
    y4 = y1 + y3 - y2;
    printf("B': [%lf,%lf], %s\n", x4, y4, get_parallelogram_type(get_distance(x1, y1, x3, y3), get_distance(x2, y2, x4, y4),
                                                                get_distance(x1, y1, x2, y2), get_distance(x2, y2, x3, y3),
                                                                x1, y1, x3, y3, x2, y2, x4, y4));

    //------Variant for AC'BC------
    x4 = x1 + x2 - x3;
    y4 = y1 + y2 - y3;
    printf("C': [%lf,%lf], %s\n", x4, y4, get_parallelogram_type(get_distance(x1, y1, x2, y2), get_distance(x3, y3, x4, y4),
                                                                 get_distance(x1, y1, x3, y3), get_distance(x3, y3, x2, y2),
                                                                 x1, y1, x2, y2, x3, y3, x4, y4));

    return 0;
}