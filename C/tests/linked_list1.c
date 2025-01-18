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
    Person * newPerson = (Person *) malloc(sizeof(Person));
    newPerson->age = age;
    newPerson->id = id;
    newPerson->next = NULL;
    strncpy(newPerson->name, name, 20);
    return newPerson;
}

void create_person_new(List * list, const size_t id, const size_t age, const char name[])
{   
    Person *tmp = NULL;
    //get head if it exists
    if (list->head != NULL)
        tmp = list->head;

    list->head = create_person(id, age, name);
    list->head->next = tmp;
}

void print_linked_list(const List *list)
{
    Person *loop = list->head;

    while (true)
    {
        if (loop == NULL)
            break;
        printf("id: %ld, age: %ld, name: %s\n", loop->age, loop->age, loop->name);
        loop = loop->next;
    }
}

void list_mem_clear(List *list)
{
    Person *tmp;

    while (true)
    {   
        tmp = list->head;
        if (tmp == NULL)
            break;
        list->head = list->head->next;
        free(tmp);
    }
}


int main()
{
    List theList;
    theList.head = NULL;
    theList.length = 0;

    create_person_new(&theList,1, 22, "Vaclav Horcicka");
    create_person_new(&theList,2, 24, "Test object");
    create_person_new(&theList,3, 30, "David Nevim");
    create_person_new(&theList,4, 20, "Ja neviem uz");


    //print the linked list
    print_linked_list(&theList);
    printf("Debil\n");
    print_linked_list(&theList);

    list_mem_clear(&theList);

    printf("the end\n");
}
