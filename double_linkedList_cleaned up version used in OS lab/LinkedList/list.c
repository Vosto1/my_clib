#include "list.h"
#include <stdlib.h>
#include <assert.h>


/*Hjalpfunktion till add
  Allokerar minne for en ny nod
  om allokeringen lyckades initieras data samt pekare (pekare initieras till NULL).
  Den nya noden (eller NULL) returneras.*/
static struct node* createListNode(const Data data)
{
    //List �r en node ptr
    List tempNodeptr = (List)malloc(sizeof(struct node));
    if (tempNodeptr != NULL)
    {
        tempNodeptr->data = data;
        tempNodeptr->next = NULL;
        tempNodeptr->previous = NULL;
        return tempNodeptr;
    }
    else
    {
        printf("Error allocating memory.\n");
        return NULL;
    }
}

/*Returnera en tom lista*/
List createEmptyList(void)
{
    return NULL;
}


/*Ar listan tom?
  Returnerar true om den �r tom, annars false*/
bool isEmpty(const List list)
{
    if (list == NULL)
        return true;
    return false;
}

/*Lagg till nod forst i listan*/
void addFirst(List* list, const Data data)
{
    //skapa ny nod
    List temp = createListNode(data);
    if (temp != NULL)
    {
        if (*list == NULL)
        {
            *list = temp;
        }
        else
        {
            List tempPtr = *list;
            *list = temp;
            (*list)->next = tempPtr;
            tempPtr->previous = *list;
        }

    }
    else
    {
        return;
    }
    assert(!isEmpty(*list));
}

/*Lagg till nod sist i listan*/
void addLast(List* list, const Data data)
{
    if ((*list) == NULL)
    {
        addFirst(list, data);
    }
    else if ((*list)->next == NULL)
    {
        List temp = createListNode(data);
        if (temp != NULL)
        {
            (*list)->next = temp;
            (*list)->next->previous = *list;
        }
    }
    else
    {
        addLast(&(*list)->next, data);
    }
}

/*Ta bort forsta noden i listan*/
void removeFirst(List* list)
{
    assert(!isEmpty(*list)); //kommentera f�r att menyn ska fungera
    if (*list != NULL)
    {
        List temp = (*list);
        (*list) = (*list)->next;
        if((*list) != NULL)
            (*list)->previous = NULL;
        free(temp);
    }
}

/*Ta bort sista noden i listan*/
void removeLast(List* list)
{
    assert(!isEmpty(*list));
    if ((*list)->next == NULL)
    {
        if ((*list)->previous != NULL) {
            List temp = (*list)->previous;
            temp->next = NULL;
        }
        free((*list));
        *list = NULL;
    }
    else
    {
        removeLast(&(*list)->next);
    }
}


bool compareStructures(Data struct1, Data struct2) {
    if( struct1.life == struct2.life &&
        struct1.mass == struct2.mass &&
        !strcmp(struct1.name, struct2.name) &&
        !strcmp(struct1.pid, struct2.pid) &&
        struct1.sx == struct2.sx &&
        struct1.sy == struct2.sy &&
        struct1.vx == struct2.vx &&
        struct1.vy == struct2.vy) {
        return true;
    }
    return false;
}

/*Ta bort data ur listan (forsta forekomsten)
  Returnera true om datat finns, annars false*/
bool removeElement(List* list, const Data data)
{
    if ((*list) != NULL)
    {
        if (/* (*list)->data == data */compareStructures((*list)->data, data))
        {
            List temp = (*list);
            (*list) = (*list)->next;
            if((*list) != NULL)
                (*list)->previous = temp->previous;
            free(temp);
            return true;
        }
        else
        {
            return removeElement(&(*list)->next, data);
        }
    }
    else
        return false;
}

/*search for data in the list
Om datat finns returneras true, annars false*/
bool search(const List list, const Data data/* const ID id */)
{
    if (list != NULL)
    {
        if (/* list->data == data */compareStructures(list->data, data))
        {
            return true;
        }
        search(list->next, data);
    }
    else
    {
        return false;
    }
}

/*Returnera antalet noder i listan*/
int numberOfNodesInList(const List list)
{
    if (list == NULL)
    {
        return 0;
    }
    else
    {
        return 1 + numberOfNodesInList(list->next); //Ersatt med ratt returvarde
    }
}

// removes dead planet nodes in the list
void removeDead(List *list) {
    if (*list != NULL)
    {
        if ((*list)->data.life <= 0)
        {
            List temp = (*list);
            (*list) = (*list)->next;
            if((*list) != NULL)
                (*list)->previous = temp->previous;
            free(temp);
            removeDead(list);
        }
        else {
            removeDead(&(*list)->next);
        }
        
    }
    else
        return;
}

/*Ta bort alla noder ur listan
  Glom inte att frigora minnet
  Postcondition: Listan ar tom, *list �r NULL (testa med assert)*/
void clearList(List* list)
{
    if((*list)->next == NULL) {
        return;
    }
    else {
        clearList(&(*list)->next);
    }
    free((*list));
    (*list) = NULL;
    assert(*list == NULL);
}

/*Skriv ut listan
  Vid anropet kan man ange stdout som argument 2 for att skriva ut p� skarmen.
  Anvanda fprintf for att skriva ut.
  Den har typen av utskriftfunktion blir mer generell da man kan valja att skriva ut till skarmen eller till fil.*/
/* void printList(const List list, FILE* textfile)
{
    if (list != NULL)
    {
        if (list->next == NULL)
        {
            fprintf(textfile, "%d", list->data);
            return;
        }
        fprintf(textfile, "%d", list->data);
        printList(list->next, textfile);
    }
} */

/*Returnera forsta datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
Data getFirstElement(const List list)
{
    assert(!isEmpty(list)); //kommentera f�r att menyn ska fungera
    if (list != NULL)
        return list->data; //Ersatt med ratt returvarde
}

/*Returnera sista datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
Data getLastElement(const List list)
{
    assert(!isEmpty(list)); //kommentera f�r att menyn ska fungera
    if (list != NULL)
    {
        if (list->next == NULL)
        {
            return list->data;
        }
        return getLastElement(list->next); //Ersatt med ratt returvarde
    }
}

/* Data getSumOfListData(List list) {
    if (list == NULL)
    {
        return 0;
    }
    else
    {
        return list->data + getSumOfListData(list->next); //Ersatt med ratt returvarde
    }
} */
