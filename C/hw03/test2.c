#include <stdio.h>

struct Date
{
    int year;
    int month;
    int day;
};

// Function to check if a year is a leap year
int isLeapYear(int year)
{
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

// Days in each month, for both regular and leap years
int daysInMonth(int year, int month)
{
    int daysInMonths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year))
    {
        return 29; // February has 29 days in a leap year
    }
    return daysInMonths[month - 1];
}

// Function to add days to a date
void addDays(struct Date *date, int daysToAdd)
{
    while (daysToAdd > 0)
    {
        int daysInCurrentMonth = daysInMonth(date->year, date->month);

        if (date->day + daysToAdd <= daysInCurrentMonth)
        {
            date->day += daysToAdd;
            daysToAdd = 0; // Done adding days
        }
        else
        {
            daysToAdd -= (daysInCurrentMonth - date->day + 1);
            date->day = 1;

            // Move to the next month
            if (date->month == 12)
            {
                date->month = 1;
                date->year++;
            }
            else
            {
                date->month++;
            }
        }
    }
}

// Function to print the date
void printDate(const struct Date *date)
{
    printf("%04d-%02d-%02d\n", date->year, date->month, date->day);
}

int main()
{
    struct Date date = {2002, 12, 1};
    int daysToAdd = 98499;

    printf("Original Date: ");
    printDate(&date);

    addDays(&date, daysToAdd);

    printf("New Date: ");
    printDate(&date);

    return 0;
}
