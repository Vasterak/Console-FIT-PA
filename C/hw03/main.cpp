#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

//these are just binary representation of each day. IDK why he choose this way in high level language but ok, like enums also exists... I'm not manipulating AVR ports...
constexpr unsigned DOW_MON      = 0b0000'0001; // 1
constexpr unsigned DOW_TUE      = 0b0000'0010; // 2
constexpr unsigned DOW_WED      = 0b0000'0100; // 4
constexpr unsigned DOW_THU      = 0b0000'1000; // 8
constexpr unsigned DOW_FRI      = 0b0001'0000; // 16
constexpr unsigned DOW_SAT      = 0b0010'0000; // 32
constexpr unsigned DOW_SUN      = 0b0100'0000; // 64
constexpr unsigned DOW_WORKDAYS = DOW_MON | DOW_TUE | DOW_WED | DOW_THU | DOW_FRI;  // 31
constexpr unsigned DOW_WEEKEND  = DOW_SAT | DOW_SUN;                                // 96
constexpr unsigned DOW_ALL      = DOW_WORKDAYS | DOW_WEEKEND;                       // 127

typedef struct TDate
{
    unsigned m_Year;
    unsigned m_Month;
    unsigned m_Day;
} TDATE;

TDATE makeDate (unsigned y, unsigned m, unsigned d )
{
    TDATE res = { y, m, d };
    return res;
}
#endif /* __PROGTEST__ */

//------------------------------------------------------MY VARIABLES-------------------------------------------
unsigned int howMuchEachDayInWeek[8] = {0}; // function will fill up this array with values that determines 'perWorkDay' to each day in week, last value is for TOTAL
const unsigned int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//------------------------------------------------------HELP FUNCTIONS-------------------------------------------

//Check for LEAP years in Gregorian calendar
bool is_leap_year(unsigned int y)
{
    if (y % 4000 == 0)
        return false;

    return !(y % 4) && ((y % 100) || !(y % 400));
}

//check if input date is valid
bool is_valid_date(TDATE date)
{
    // Year cannot be less than 2000
    if (date.m_Year < 2000)
        return false;

    // Months only 1-12
    if (date.m_Month < 1 || date.m_Month > 12)
        return false;

    // Days only 1-31
    if (date.m_Day > 31 || date.m_Day < 1)
        return false;

    if (date.m_Day > 29 && date.m_Month == 2)
        return false;

    // Check 29 on February
    if (date.m_Day == 29 && date.m_Month == 2)
    {
        // If this is not leap year return false.
        if (!is_leap_year(date.m_Year))
            return false;
    }

    // 'Even' months 2,4,6,9,11 cannot have 31 days
    if (((date.m_Month % 2 == 0) && date.m_Month < 7) || date.m_Month == 9 || date.m_Month == 11)
    {
        if (date.m_Day > 30)
            return false;
    }

    return true;
}

// return days in specific zellers index, the _means it is internal fuction and should't be call randomly
int _zellers_algorithm(TDATE date)
{
    int year2 = date.m_Year;
    int m = date.m_Month;
    // Janury is 13 and February is 14
    if (m == 1)
    {
        m = 13;
        year2--;
    }
    else if (m == 2)
    {
        m = 14;
        year2--;
    }

    int K = year2 % 100; // year of the century
    int J = year2 / 100; // is the zero-based century for 1995 it is 19

    // https://en.wikipedia.org/wiki/Zeller%27s_congruence
    return (date.m_Day + ((13 * (m + 1)) / 5) + K + (K / 4) + (J / 4) + (5 * J)) % 7;
}

//return the ISO index for each day, Monday = 0, Tuesday = 2 ... Sunday = 6.
int what_day_is_it(TDATE date)
{
    return ((_zellers_algorithm(date) + 5 ) % 7);
}

//calculate total connections per week for each day
void calc_connections_week(const unsigned dowMask, const unsigned perWorkDay)
{
    //clear the array
    for (size_t i = 0; i < sizeof(howMuchEachDayInWeek) / sizeof(unsigned int); i++)
        howMuchEachDayInWeek[i] = 0;
    
    //set values to it
    for (size_t i = 0; i < (sizeof(howMuchEachDayInWeek) / sizeof(unsigned int) - 1); i++)
    {
        //check if bit is set for that position
        if (dowMask & (1 << i))
        {
            //saturday
            if (i == 5)
                howMuchEachDayInWeek[i] = ceil(perWorkDay / 2.f);

            //sunday
            else if (i == 6)
                howMuchEachDayInWeek[i] = ceil(perWorkDay / 3.f);

            //workweek
            else
                howMuchEachDayInWeek[i] = perWorkDay;
        }
        //get total
        howMuchEachDayInWeek[7] += howMuchEachDayInWeek[i];
    }
}

bool is_same_date(TDATE from, TDATE to)
{
    if (from.m_Day == to.m_Day && from.m_Month == to.m_Month && from.m_Year == to.m_Year)
        return true;
    else
        return false;
}

//used for total days counting, internal function do not use independelty
unsigned _count_leap_years(TDATE date)
{
    unsigned years = date.m_Year;

    if (date.m_Month <= 2)
        years--;

    return years / 4 - years / 100 + years / 400;
}

// return number of days between two dates
unsigned int get_difference(TDATE from, TDATE to)
{
    unsigned long long n1 = from.m_Year * 365 + from.m_Day;

    for (size_t i = 0; i < from.m_Month - 1; i++)
        n1 += daysInMonth[i];

    n1 += _count_leap_years(from);

    unsigned long long n2 = to.m_Year * 365 + to.m_Day;

    for (size_t i = 0; i < to.m_Month - 1; i++)
        n2 += daysInMonth[i];

    n2 += _count_leap_years(to);

    // return difference between two counts
    return (n2 - n1);
}

//------------------------------------------------------MAIN FUNCTIONS-------------------------------------------

long long countConnections ( TDATE from, TDATE to, unsigned  perWorkDay, unsigned  dowMask )
{
    //check input for valid data
    if (!is_valid_date(from))
        return -1;
    
    if (!is_valid_date(to))
        return -1;


    // Check if first date is lower than second date, dates can be equal
    if (to.m_Year < from.m_Year)
        return -1;

    if (to.m_Year == from.m_Year)
    {
        if (to.m_Month < from.m_Month)
            return -1;

        if (to.m_Month == from.m_Month)
        {
            if (to.m_Day < from.m_Day)
                return -1;
        }
    }

    //CALCULATIONS
    
    //variables
    TDATE workDate;
    workDate.m_Day = from.m_Day;
    workDate.m_Month = from.m_Month;
    workDate.m_Year = from.m_Year;

    unsigned int totalDays = 0;
    unsigned int firstDayIndex = what_day_is_it(from);
    unsigned int totalConnections = 0;

    //set new values for week
    calc_connections_week(dowMask, perWorkDay);

    //get total days betweem dates
    totalDays = get_difference(from, to) + 1;

    // debug
    printf("firstDayIndex %d\n", firstDayIndex); //what day is the start date
    printf("totalDays %d\n",     totalDays);    // how many days are there



    //calculate to begining of the week
    for (size_t i = firstDayIndex; i < (sizeof(howMuchEachDayInWeek) / sizeof(unsigned int) - 1); i++)
    {
        if (totalDays == 0)
        {
            printf("totalConnections %d\n", totalConnections);
            printf("------------------\n");
            return totalConnections;
        }
        
        //check if for that day connection was done. if it wasn't there is stored 0 inside array, so no need for if statement
        totalConnections += howMuchEachDayInWeek[i];
        totalDays--;
    }

    //Calculate the number of connections for each day.
    while (totalDays != 0)
    {        
        //goes though one week
        for (size_t i = 0; i < (sizeof(howMuchEachDayInWeek) / sizeof(unsigned int) - 1); i++)
        {
            if (totalDays == 0)
            {
                printf("totalConnections %d\n", totalConnections);
                printf("------------------\n");

                return totalConnections;
            }

            totalConnections += howMuchEachDayInWeek[i];
            totalDays--;
        }
    }
    return totalConnections;
}

TDATE endDate ( TDATE from, long long connections, unsigned  perWorkDay, unsigned  dowMask )
{
    TDATE test;
    test.m_Day = 5;
    test.m_Month = 10;
    test.m_Year = 100;

    return test;
}

#ifndef __PROGTEST__
int main ()
{
    TDATE d;
    assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 31), 1, DOW_ALL) == 31);
    assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 31), 10, DOW_ALL) == 266);
    assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 31), 1, DOW_WED) == 5);
    assert(countConnections(makeDate(2024, 10, 2), makeDate(2024, 10, 30), 1, DOW_WED) == 5);
    assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 1), 10, DOW_TUE) == 10);
    assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 1), 10, DOW_WED) == 0);
    assert(countConnections(makeDate(2024, 1, 1), makeDate(2034, 12, 31), 5, DOW_MON | DOW_FRI | DOW_SAT) == 7462);
    assert(countConnections(makeDate(2024, 1, 1), makeDate(2034, 12, 31), 0, DOW_MON | DOW_FRI | DOW_SAT) == 0);
    assert(countConnections(makeDate(2024, 1, 1), makeDate(2034, 12, 31), 100, 0) == 0);
    assert(countConnections(makeDate(2024, 10, 10), makeDate(2024, 10, 9), 1, DOW_MON) == -1);
    assert(countConnections(makeDate(2024, 2, 29), makeDate(2024, 2, 29), 1, DOW_ALL) == 1);
    assert(countConnections(makeDate(2023, 2, 29), makeDate(2023, 2, 29), 1, DOW_ALL) == -1);
    assert(countConnections(makeDate(2100, 2, 29), makeDate(2100, 2, 29), 1, DOW_ALL) == -1);
    assert(countConnections(makeDate(2400, 2, 29), makeDate(2400, 2, 29), 1, DOW_ALL) == 1);
    assert(countConnections(makeDate(4000, 2, 29), makeDate(4000, 2, 29), 1, DOW_ALL) == -1);

    d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_ALL);
    assert(d.m_Year == 2025 && d.m_Month == 1 && d.m_Day == 8);
    d = endDate(makeDate(2024, 10, 1), 100, 6, DOW_ALL);
    assert(d.m_Year == 2024 && d.m_Month == 10 && d.m_Day == 20);
    d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_WORKDAYS);
    assert(d.m_Year == 2025 && d.m_Month == 2 && d.m_Day == 17);
    d = endDate(makeDate(2024, 10, 1), 100, 4, DOW_WORKDAYS);
    assert(d.m_Year == 2024 && d.m_Month == 11 && d.m_Day == 4);
    d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_THU);
    assert(d.m_Year == 2026 && d.m_Month == 9 && d.m_Day == 2);
    d = endDate(makeDate(2024, 10, 1), 100, 2, DOW_THU);
    assert(d.m_Year == 2025 && d.m_Month == 9 && d.m_Day == 17);
    d = endDate(makeDate(2024, 10, 1), 100, 0, DOW_THU);
    assert(d.m_Year == 0 && d.m_Month == 0 && d.m_Day == 0);
    d = endDate(makeDate(2024, 10, 1), 100, 1, 0);
    assert(d.m_Year == 0 && d.m_Month == 0 && d.m_Day == 0);
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
