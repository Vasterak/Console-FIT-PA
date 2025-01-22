#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


typedef struct person
{
    size_t id;
    char firstName[10];
    char lastName[20];
    struct person * next;
} Person;

typedef struct list
{
    Person * head;
    size_t length;
} List;

char peek_char()
{
    char a = getchar();
    if (a != EOF)
        ungetc(a, stdin);
    return a;
}

void clear_whitespace()
{
    char a;
    while ((a = peek_char()) && (a != EOF) && (isspace(a)) && (a != '\n'))
        getchar();
}

//return of -1 is error state
int get_people_input()
{   
    int scanfout, number;
    scanfout = scanf(" %d", &number);

    if (scanfout != 1)
        return -1;

    if (number <= 0)
        return -1;

    clear_whitespace();
    if (!isspace(peek_char()))
    {
        printf("you have input a non valid character after your number!\n");
        return -1;
    }

    return number;
}

//generates "random" char between a-z
char random_character()
{
    char a = 'a' + rand() % 25;
    return a;
}

void generate_name(char * input, const size_t length)
{
    for (size_t i = 0; i < length - 1; i++)
        input[i] = random_character();

    input[length - 1] = '\0';
}

Person * create_person(const size_t index)
{
    Person *newPerson = (Person *) malloc(sizeof(Person));
    if (newPerson == NULL)
        return NULL;
    newPerson->id = index;
    generate_name((newPerson->firstName), 10);
    generate_name((newPerson->lastName), 20);
    newPerson->next = NULL;
    return newPerson;
}

bool generate_people(List *list, const int peopleNumber)
{
    //Create a fist person and start the list
    Person *additionToList = create_person(0);
    if (additionToList == NULL)
        return false;
    
    list->head = additionToList;

    //repeat for every other person
    for (int i = 1; i < peopleNumber; i++)
    {
        //create a new person
        additionToList = create_person(i);

        additionToList->next = list->head;
        list->head = additionToList;
    }
    return true;
}

void print_people(const List * people)
{
    Person * looping = people->head;
    while (looping->next != NULL)
    {
        printf("ID: %02ld, Name: %s, Surname: %s\n", looping->id, looping->firstName, looping->lastName);
        looping = looping->next;
    }
    
}

void clear_memory(List * list)
{
    //get the head
    Person * tmp = list->head;
    
    Person * toBeDeleted;

    while (tmp->next != NULL)
    {
        //copy the tmp address to deletion process
        toBeDeleted = tmp;
        tmp = tmp->next;
        free(toBeDeleted);
    }
    free(tmp);    
}

void sort_by_name(List * list)
{
    Person * head;
    for (size_t i = 0; i < 20; i++)
    {
        head = list->head;

        for (size_t j = 0; j < 20 - i - 1; j++)
        {
            //condition to swap
            //printf("First: %c\t Second: %c\n", first->firstName[0], first->next->firstName[0]);
            if ((head->firstName[0]) > (head->next->firstName[0]))
            {
                printf("swaped!\n");
            }
            head = head->next;
        }
    }
}
//DO NOT WORK!!!
void sort_by_name2(List * list)
{
    Person * head;
    Person * last;

    for (size_t i = 0; i < 8; i++)
    {
        head = list->head;
        for (size_t j = 0; j < 8 - i - 1; j++)
        {
            //intended to run only when the head is the beging of the array
            if (head->next->next != NULL)
            {
                if ((head->firstName[0] > head->next->firstName[0]) && j == 0)
                {
                    Person * swap1 = head;
                    Person * next_ = head->next->next;
                    head = swap1->next;
                    list->head = head; //important
                    swap1->next->next = swap1;
                    swap1->next = next_;
                }
                //in regual chain.
                else if ((head->firstName[0] > head->next->firstName[0]) && j != 0)
                {
                    Person * next_ = head->next->next;
                    last->next = head->next; // from last to the third node
                    head->next->next = head;
                    head->next = next_;
                }
            }
            //save the node before head and head->next as this node need to point to the new one.
            last = head;
            //increment to next nodes
            head = head->next;
        }
    }
}

bool sort_people(List * people)
{
    char inputChar;
    int scanfOut = scanf(" %c", &inputChar);
    if (scanfOut != 1)
        return false;
    
    inputChar = toupper(inputChar);
    switch (inputChar)
    {
    // sort by first name
    case 'N':
        sort_by_name2(people);
        printf("Sorted\n");
        break;
    // sort by last name
    case 'S':

        break;        
    
    default:
        break;
    }
    return true;
}

int main()
{
    //Get input for how many of them you want to generate
    printf("Input how many people you want to generate.\n");
    int peopleNumber = get_people_input();
    if (peopleNumber == -1)
    {
        printf("Wrong input.\n");
        return 0;
    }

    //Create a list
    List peopleList;
    peopleList.length = 0;
    peopleList.head = NULL;

    //generate them
    if (!generate_people(&peopleList, peopleNumber))
    {
        printf("There was a problem with people generation.\n");
        return 0;
    }

    //print them
    printf("Here is the generated list of: %d people.\n", peopleNumber);
    print_people(&peopleList);

    //sort them
    printf("How do you want to sort it?\n");
    printf("Enter 'N' by first name, Enter 'S' by surname\n");

    if (!sort_people(&peopleList))
    {
        printf("There was a error while sorting the people.\n");
        return 0;
    }
    printf("------------------------\n");
    print_people(&peopleList);

    

    //Clear the memory, I have to do it myself :(
    clear_memory(&peopleList);

    printf("The End\n");

    return 0;
}