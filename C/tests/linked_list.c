#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

typedef struct person
{
    //data
    size_t id;
    size_t age;
    char name[20];

    //Next node pointer
    struct person *next;

} Person; 

typedef struct list
{
    Person *head;
    size_t length;

} List;


Person * create_person(const size_t id, const size_t age, const char name[])
{
    Person * newe = (Person *) malloc(sizeof(Person));
    newe->age = age;
    newe->id = id;
    newe->next = NULL;
    strncpy(newe->name, name, 20);

    return newe;
}


int main()
{
    List theList;
    theList.length = 0;

    theList.head = create_person(1, 22, "Vaclav Horcicka");

    theList.head->next = create_person(2, 24, "Test object");

    theList.head->next->next = create_person(2, 20, "Teste object");

    Person * lastOne;
    lastOne = theList.head;
    while (true)
    {
        if (lastOne != NULL)
        {
            printf("id: %ld, age: %ld, name: %s\n", lastOne->id, lastOne->age, lastOne->name);
            lastOne = lastOne->next;
        }
        else
            break;

    }

    lastOne = theList.head;
    while (true)
    {
        Person *tmp;
        if (lastOne != NULL)
        {
            tmp = lastOne; 
            lastOne = lastOne->next;
            free(tmp);
        }
        else
            break;

    }

    printf("the end\n");
}
