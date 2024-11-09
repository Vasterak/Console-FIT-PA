#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

constexpr unsigned DOW_MON      = 0b0000'0001;
constexpr unsigned DOW_TUE      = 0b0000'0010;
constexpr unsigned DOW_WED      = 0b0000'0100;
constexpr unsigned DOW_THU      = 0b0000'1000;
constexpr unsigned DOW_FRI      = 0b0001'0000;
constexpr unsigned DOW_SAT      = 0b0010'0000;
constexpr unsigned DOW_SUN      = 0b0100'0000;
constexpr unsigned DOW_WORKDAYS = DOW_MON | DOW_TUE | DOW_WED | DOW_THU | DOW_FRI;
constexpr unsigned DOW_WEEKEND  = DOW_SAT | DOW_SUN;
constexpr unsigned DOW_ALL      = DOW_WORKDAYS | DOW_WEEKEND;

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

long long countConnections ( TDATE from, TDATE to, unsigned  perWorkDay, unsigned  dowMask )
{
    
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
  TDATE d;
  assert ( countConnections ( makeDate ( 2024, 10, 1 ), makeDate ( 2024, 10, 31 ), 1, DOW_ALL ) == 31 );
  assert ( countConnections ( makeDate ( 2024, 10, 1 ), makeDate ( 2024, 10, 31 ), 10, DOW_ALL ) == 266 );
  assert ( countConnections ( makeDate ( 2024, 10, 1 ), makeDate ( 2024, 10, 31 ), 1, DOW_WED ) == 5 );
  assert ( countConnections ( makeDate ( 2024, 10, 2 ), makeDate ( 2024, 10, 30 ), 1, DOW_WED ) == 5 );
  assert ( countConnections ( makeDate ( 2024, 10, 1 ), makeDate ( 2024, 10, 1 ), 10, DOW_TUE ) == 10 );
  assert ( countConnections ( makeDate ( 2024, 10, 1 ), makeDate ( 2024, 10, 1 ), 10, DOW_WED ) == 0 );
  assert ( countConnections ( makeDate ( 2024, 1, 1 ), makeDate ( 2034, 12, 31 ), 5, DOW_MON | DOW_FRI | DOW_SAT ) == 7462 );
  assert ( countConnections ( makeDate ( 2024, 1, 1 ), makeDate ( 2034, 12, 31 ), 0, DOW_MON | DOW_FRI | DOW_SAT ) == 0 );
  assert ( countConnections ( makeDate ( 2024, 1, 1 ), makeDate ( 2034, 12, 31 ), 100, 0 ) == 0 );
  assert ( countConnections ( makeDate ( 2024, 10, 10 ), makeDate ( 2024, 10, 9 ), 1, DOW_MON ) == -1 );
  assert ( countConnections ( makeDate ( 2024, 2, 29 ), makeDate ( 2024, 2, 29 ), 1, DOW_ALL ) == 1 );
  assert ( countConnections ( makeDate ( 2023, 2, 29 ), makeDate ( 2023, 2, 29 ), 1, DOW_ALL ) == -1 );
  assert ( countConnections ( makeDate ( 2100, 2, 29 ), makeDate ( 2100, 2, 29 ), 1, DOW_ALL ) == -1 );
  assert ( countConnections ( makeDate ( 2400, 2, 29 ), makeDate ( 2400, 2, 29 ), 1, DOW_ALL ) == 1 );
  assert ( countConnections ( makeDate ( 4000, 2, 29 ), makeDate ( 4000, 2, 29 ), 1, DOW_ALL ) == -1 );
  d = endDate ( makeDate ( 2024, 10, 1 ), 100, 1, DOW_ALL );
  assert ( d . m_Year == 2025 && d . m_Month == 1 && d . m_Day == 8 );
  d = endDate ( makeDate ( 2024, 10, 1 ), 100, 6, DOW_ALL );
  assert ( d . m_Year == 2024 && d . m_Month == 10 && d . m_Day == 20 );
  d = endDate ( makeDate ( 2024, 10, 1 ), 100, 1, DOW_WORKDAYS );
  assert ( d . m_Year == 2025 && d . m_Month == 2 && d . m_Day == 17 );
  d = endDate ( makeDate ( 2024, 10, 1 ), 100, 4, DOW_WORKDAYS );
  assert ( d . m_Year == 2024 && d . m_Month == 11 && d . m_Day == 4 );
  d = endDate ( makeDate ( 2024, 10, 1 ), 100, 1, DOW_THU );
  assert ( d . m_Year == 2026 && d . m_Month == 9 && d . m_Day == 2 );
  d = endDate ( makeDate ( 2024, 10, 1 ), 100, 2, DOW_THU );
  assert ( d . m_Year == 2025 && d . m_Month == 9 && d . m_Day == 17 );
  d = endDate ( makeDate ( 2024, 10, 1 ), 100, 0, DOW_THU );
  assert ( d . m_Year == 0 && d . m_Month == 0 && d . m_Day == 0 );
  d = endDate ( makeDate ( 2024, 10, 1 ), 100, 1, 0 );
  assert ( d . m_Year == 0 && d . m_Month == 0 && d . m_Day == 0 );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
