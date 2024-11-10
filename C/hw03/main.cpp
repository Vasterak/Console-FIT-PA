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

int what_day_is_it()
{
    
}

bool is_leap_year(int y)
{
    if (y % 4000 == 0)
        return false;

    return !(y % 4) && ((y % 100) || !(y % 400));
    // if (((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0))
    //   return true;
    // else
    //   return false;
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



    return 0;
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
    printf("%d\n", DOW_MON);
    printf("%d\n", DOW_TUE);
    printf("%d\n", DOW_WED);
    printf("%d\n", DOW_THU);
    printf("%d\n", DOW_FRI);
    printf("%d\n", DOW_SAT);
    printf("%d\n", DOW_SUN);
    printf("--------\n");
    printf("%d\n", DOW_WORKDAYS);
    printf("%d\n", DOW_WEEKEND);
    printf("%d\n", DOW_ALL);

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
