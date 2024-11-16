
#include <stdio.h>
#include <assert.h>

// A date has day 'd', month 'm' and year 'y'
typedef struct TDate
{
    unsigned m_Year;
    unsigned m_Month;
    unsigned m_Day;
} TDATE;

typedef struct
{
    int m_TotalDays;
} TResult;

// To store number of days in
// all months from January to Dec.
const unsigned int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// This function counts number of
// leap years before the given date
unsigned count_leap_years(TDATE date)
{
    int years = date.m_Year;

    if (date.m_Month <= 2)
        years--;

    return years / 4 - years / 100 + years / 400;
}

// This function returns number of
// days between two given dates
unsigned int get_difference(TDATE from, TDATE to)
{
    unsigned long long n1 = from.m_Year * 365 + from.m_Day;

    for (size_t i = 0; i < from.m_Month - 1; i++)
        n1 += daysInMonth[i];

    n1 += count_leap_years(from);

    unsigned long long n2 = to.m_Year * 365 + to.m_Day;

    for (size_t i = 0; i < to.m_Month - 1; i++)
        n2 += daysInMonth[i];

    n2 += count_leap_years(to);

    // return difference between two counts
    return (n2 - n1);
}

TResult countDays(unsigned y1, unsigned m1, unsigned d1, unsigned y2, unsigned m2, unsigned d2)
{
    TResult result;
    result.m_TotalDays = -1;

    TDATE from = {y1, m1, d1};
    TDATE to = {y2, m2, d2};

    result.m_TotalDays = get_difference(from, to) + 1;
    printf("total: %d\n", result.m_TotalDays);

    return result;
}

// Driver code
int main()
{
    TResult r;

    r = countDays(2023, 11, 1,
                  2023, 11, 30);
    assert(r.m_TotalDays == 30);

    r = countDays(2000, 1, 1,
                  2000, 3, 1);
    assert(r.m_TotalDays == 61);

    r = countDays(2000, 1, 1,
                  2000, 1, 30);
    assert(r.m_TotalDays == 30);

    r = countDays(2000, 2, 29,
                  2000, 2, 29);
    assert(r.m_TotalDays == 1);

    r = countDays(2000, 1, 1,
                  2000, 1, 2);
    assert(r.m_TotalDays == 2);

    r = countDays(2023, 11, 1,
                  2023, 11, 17);
    assert(r.m_TotalDays == 17);

    r = countDays(2023, 11, 1,
                  2023, 11, 1);
    assert(r.m_TotalDays == 1);

    r = countDays(2024, 2, 29,
                  2024, 2, 29);
    assert(r.m_TotalDays == 1);

    r = countDays(2023, 11, 17,
                  2023, 11, 17);
    assert(r.m_TotalDays == 1);

    r = countDays(2023, 1, 1,
                  2023, 12, 31);
    assert(r.m_TotalDays == 365);

    r = countDays(2024, 1, 1,
                  2024, 12, 31);
    assert(r.m_TotalDays == 366);

    r = countDays(2000, 1, 1,
                  2023, 12, 31);
    assert(r.m_TotalDays == 8766);

    r = countDays(2001, 2, 3,
                  2023, 7, 18);
    assert(r.m_TotalDays == 8201);

    r = countDays(2021, 3, 31,
                  2023, 11, 12);
    assert(r.m_TotalDays == 957);

    return 0;
}
