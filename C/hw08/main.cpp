#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

constexpr int PHONE_DIGITS = 10;

typedef struct TNode
{
    char * m_Name;
    struct TNode * m_Child[PHONE_DIGITS];
} TNODE;

typedef struct
{
    TNODE * m_Root;
    int m_Size;
} TPHONEBOOK;

#endif /* __PROGTEST__ */

// -------------------- MY FUNCTIONS ------------------------

//check if input string (phone number) contains only valid characters 0-9
bool is_valid_phone(const char * phoneNumber)
{
    if (strlen(phoneNumber) <= 0)
        return false;

    size_t index = 0;
    while (phoneNumber[index] != '\0')
    {
        if (phoneNumber[index] < '0' || phoneNumber[index] > '9')
            return false;
        index++;
    }
    return true;
}

TNODE* create_node_init()
{
    TNODE * node = (TNODE*) malloc(sizeof(TNODE));
    if (!node)
        return nullptr;

    node->m_Name = nullptr;
    
    for (size_t i = 0; i < PHONE_DIGITS; i++)
        node->m_Child[i] = nullptr;

    return node;
}

static TNODE *create_node()
{
    TNODE *newNode = (TNODE *)malloc(sizeof(TNODE));

    if (newNode == nullptr)
        return nullptr;

    newNode->m_Name = nullptr;

    for (int i = 0; i < 10; i++)
        newNode->m_Child[i] = nullptr;
    return newNode;
}

// not performed directly in "delBook" because of TNODE argument.
void free_phonebook(TNODE * currentRoot)
{
    if (currentRoot == nullptr)
        return;

    //loop though the array 0-9
    for (size_t i = 0; i < PHONE_DIGITS; i++)
    {
        if (currentRoot->m_Child[i] == nullptr)
            continue;
        
        //go deeper to next node and repeat.
        free_phonebook(currentRoot->m_Child[i]);
        currentRoot->m_Child[i] = nullptr;
    }
    
    //free the name if it existed
    if (currentRoot->m_Name != nullptr)
    {
        free(currentRoot->m_Name);
        currentRoot->m_Name = nullptr; //does this need to be here?
    }

    free(currentRoot);
}

static int is_node_empty(const TNODE *node)
{
    if (!node)
        return 1;

    if (node->m_Name)
        return 0;

    for (int i = 0; i < 10; i++)
    {
        if (node->m_Child[i] != nullptr)
            return 0;
    }

    return 1;
}

static TNODE *remove_phone_recursive(TNODE *current, const char *phone, TPHONEBOOK *book, int *removed)
{
    if (current == nullptr)
        return nullptr;

    if (!*phone)
    {
        // in final prefix
        if (current->m_Name)
        {
            // delete it and chage size
            free(current->m_Name);
            current->m_Name = nullptr;
            book->m_Size--;
            *removed = 1;
        }
    }
    else
    {
        int digit = *phone - '0';
        current->m_Child[digit] = remove_phone_recursive(current->m_Child[digit], phone + 1, book, removed);
    }

    // check for empty
    if (is_node_empty(current))
    {
        free(current);
        current = NULL;
    }

    return current;
}

// -------------------- PRE-DEFINED FUNCTIONS ------------------------

bool addPhone( TPHONEBOOK * book, const char * phone, const char * name )
{
    //input check
    if (book == nullptr || name == nullptr)
        return false;

    if (!is_valid_phone(phone))
        return false;

    if (!book->m_Root)
        book->m_Root = create_node();

    TNODE *currentNode = book->m_Root;

    //go digit by digit
    for (const char *p = phone; *p; p++)
    {
        int digit = *p - '0';
        if (!currentNode->m_Child[digit])
            currentNode->m_Child[digit] = create_node();
        
        currentNode = currentNode->m_Child[digit];
    }

    //size_t nameLength = strlen(name);

    //loop finished and apply name if the name does not exists
    if (currentNode->m_Name == nullptr)
    {
        //alloc memory for the input name and copy it.
        currentNode->m_Name = (char *)malloc(strlen(name) + 1);

        strcpy(currentNode->m_Name, name);
        book->m_Size++;
        //printf("name is: %s\n", name);
    }
    //When name already exists
    else
    {
        //alloc new memory for new name free the existing name and replace it. or maybe realloc will be better? idk just the preference.
        char *tmp = (char *)malloc(strlen(name) + 1);

        strcpy(tmp, name);
        free(currentNode->m_Name);
        currentNode->m_Name = tmp;
        //printf("name rewrite: %s\n", name);
    }

    return true;
}

void delBook ( TPHONEBOOK * book )
{
    book->m_Size = 0;
    free_phonebook(book->m_Root);
    book ->m_Root = nullptr;
}

bool delPhone ( TPHONEBOOK * book, const char * phone )
{
    if (!book || !phone)
        return false;

    if (!is_valid_phone(phone))
        return false;
        
    if (!book->m_Root)
        return false;

    int removed = 0;
    book->m_Root = remove_phone_recursive(book->m_Root, phone, book, &removed);
    return removed ? true : false;
}

const char * findPhone ( TPHONEBOOK * book, const char * phone )
{
    char * bestMatchName = nullptr;

    // input check
    if (book == nullptr || !is_valid_phone(phone) || book->m_Root == nullptr)
        return nullptr;

    TNODE *currentNode = book->m_Root;

    //silimar as in add phone
    for (const char *p = phone; *p; p++)
    {
        // if node has the name save it
        if (currentNode->m_Name)
            bestMatchName = currentNode->m_Name;

        int digit = *p - '0';

        // check if next number exists in the tree if not end and return the best name so far.
        if (!currentNode->m_Child[digit])
            break;

        currentNode = currentNode->m_Child[digit];
    }

    //make sure when the loop exits, that the possible last node is checked, if it contains name it is valid.
    if (currentNode->m_Name != nullptr)
        bestMatchName = currentNode->m_Name;

    return bestMatchName;
}

#ifndef __PROGTEST__

int main ()
{
  TPHONEBOOK b = { nullptr, 0 };
  char tmpStr[100];
  const char * name;
  assert ( addPhone ( &b, "420", "Czech Republic" ) );
  assert ( addPhone ( &b, "42022435", "Czech Republic CVUT" ) );
  assert ( addPhone ( &b, "421", "Slovak Republic" ) );
  assert ( addPhone ( &b, "44", "Great Britain" ) );
  strncpy ( tmpStr, "USA", sizeof ( tmpStr ) - 1 );
  assert ( addPhone ( &b, "1", tmpStr ) );
  strncpy ( tmpStr, "Guam", sizeof ( tmpStr ) - 1 );
  assert ( addPhone ( &b, "1671", tmpStr ) );
  assert ( addPhone ( &b, "44", "United Kingdom" ) );
  assert ( b . m_Size == 6 );
  assert ( ! b . m_Root -> m_Name );
  assert ( ! b . m_Root -> m_Child[0] );
  assert ( ! strcmp ( b . m_Root -> m_Child[1] -> m_Name, "USA" ) );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Name );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Name );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[0] );
  assert ( ! strcmp ( b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Name, "Guam" ) );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Name );
  assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Name, "Czech Republic" ) );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[4] );
  assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Name, "Czech Republic CVUT" ) );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[9] );
  assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Name, "Slovak Republic" ) );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[3] );
  assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[4] -> m_Name, "United Kingdom" ) );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[9] );
  name = findPhone ( &b, "420800123456" );  assert ( !strcmp ( name, "Czech Republic" ) );
  name = findPhone ( &b, "420224351111" );  assert ( !strcmp ( name, "Czech Republic CVUT" ) );
  name = findPhone ( &b, "42022435" );  assert ( !strcmp ( name, "Czech Republic CVUT" ) );
  name = findPhone ( &b, "4202243" );  assert ( !strcmp ( name, "Czech Republic" ) );
  name = findPhone ( &b, "420224343258985224" );  assert ( !strcmp ( name, "Czech Republic" ) );
  name = findPhone ( &b, "42" );  assert ( !name );
  name = findPhone ( &b, "422" );  assert ( !name );
  name = findPhone ( &b, "4422" );  assert ( !strcmp ( name, "United Kingdom" ) );
  name = findPhone ( &b, "16713425245763" );  assert ( !strcmp ( name, "Guam" ) );
  name = findPhone ( &b, "123456789123456789" );  assert ( !strcmp ( name, "USA" ) );
  assert ( delPhone ( &b, "420" ) );
  assert ( delPhone ( &b, "421" ) );
  assert ( delPhone ( &b, "44" ) );
  assert ( delPhone ( &b, "1671" ) );
  assert ( !delPhone ( &b, "1672" ) );
  assert ( !delPhone ( &b, "1671" ) );
  name = findPhone ( &b, "16713425245763" );  assert ( !strcmp ( name, "USA" ) );
  name = findPhone ( &b, "4422" );  assert ( !name );
  name = findPhone ( &b, "420800123456" );  assert ( !name );
  name = findPhone ( &b, "420224351111" );  assert ( !strcmp ( name, "Czech Republic CVUT" ) );
  assert ( b . m_Size == 2 );
  assert ( ! b . m_Root -> m_Name );
  assert ( ! b . m_Root -> m_Child[0] );
  assert ( ! strcmp ( b . m_Root -> m_Child[1] -> m_Name, "USA" ) );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[4] );
  assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Name, "Czech Republic CVUT" ) );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[9] );
  assert ( delPhone ( &b, "1" ) );
  assert ( delPhone ( &b, "42022435" ) );
  assert ( !addPhone ( &b, "12345XYZ", "test" ) );
  assert ( b . m_Size == 0 );
  assert ( ! b . m_Root );

  delBook ( &b );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
