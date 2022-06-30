#include "list.h"

/*Hjalpfunktion till add
  Allokerar minne for en ny nod
  om allokeringen lyckades initieras data samt pekare (pekare initieras till NULL).
  Den nya noden (eller NULL) returneras.*/
static struct node *createListNode(const Data data)
{
    // List �r en node ptr
    List tempNodeptr = (List)malloc(sizeof(struct node));
    if (tempNodeptr != NULL)
    {
        tempNodeptr->data = data;
        tempNodeptr->next = NULL;
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
  Returnerar 1 om den �r tom, annars 0*/
int isEmpty(const List list)
{
    if (list == NULL)
        return 1;
    return 0;
}

/*Lagg till nod forst i listan*/
void addFirst(List *list, const Data data)
{
    // skapa ny nod
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
        }
    }
    else
    {
        return;
    }
    assert(!isEmpty(*list));
}

/*Lagg till nod sist i listan*/
void addLast(List *list, const Data data)
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
        }
    }
    else
    {
        addLast(&(*list)->next, data);
    }
}

/*Ta bort forsta noden i listan*/
void removeFirst(List *list)
{
    assert(!isEmpty(*list)); // kommentera f�r att menyn ska fungera
    if (*list != NULL)
    {
        List temp = (*list);
        (*list) = (*list)->next;
        free(temp);
    }
    else
    {
        printf("List is empty.\n");
    }
}

/*Ta bort sista noden i listan*/
void removeLast(List *list)
{
    assert(!isEmpty(*list));
    if ((*list)->next == NULL)
    {
        free((*list));
        *list = NULL;
    }
    else
    {
        removeLast(&(*list)->next);
    }
}

/*Ta bort data ur listan (forsta forekomsten)
  Returnera 1 om datat finns, annars 0*/
int removeElement(List *list, const Data data)
{
    if ((*list) != NULL)
    {
        if ((*list)->data == data)
        {
            List temp = (*list);
            (*list) = (*list)->next;
            free(temp);
            return 1;
        }
        else
        {
            return removeElement(&(*list)->next, data);
        }
    }
    else
        return 0;
}

/*search for data in the list
Om datat finns returneras 1, annars 0*/
int search(const List list, const Data data)
{
    if (list != NULL)
    {
        if (list->data == data)
        {
            return 1;
        }
        else if (list->data != data && list->next == NULL)
        {
            return 0;
        }
        return search(list->next, data);
    }
    else
    {
        return 0;
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
        return 1 + numberOfNodesInList(list->next); // Ersatt med ratt returvarde
    }
}

/*Ta bort alla noder ur listan
  Glom inte att frigora minnet
  Postcondition: Listan ar tom, *list �r NULL (testa med assert)*/
void clearList(List *list)
{
    if ((*list) == NULL)
    {
        free((*list));
        (*list) = NULL;
        assert(*list == NULL);
        return;
    }
    clearList(&(*list)->next);
    free((*list));
    (*list) = NULL;
}

/*Skriv ut listan
  Vid anropet kan man ange stdout som argument 2 for att skriva ut p� skarmen.
  Anvanda fprintf for att skriva ut.
  Den har typen av utskriftfunktion blir mer generell da man kan valja att skriva ut till skarmen eller till fil.*/
/*void printList(const List list, FILE *textfile)
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
}*/

/*Returnera forsta datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
Data getFirstElement(const List list)
{
    assert(!isEmpty(list)); // kommentera f�r att menyn ska fungera
    if (list != NULL)
        return list->data; // Ersatt med ratt returvarde
    return -1; // cant happen due to assert
}

/*Returnera sista datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
Data getLastElement(const List list)
{
    assert(!isEmpty(list)); // kommentera f�r att menyn ska fungera
    if (list != NULL)
    {
        if (list->next == NULL)
        {
            return list->data;
        }
        return getLastElement(list->next); // Ersatt med ratt returvarde
    }
    return -1; // cant happen due to assert
}

Data getSumOfListData(List list)
{
    if (list == NULL)
    {
        return 0;
    }
    else
    {
        return list->data + getSumOfListData(list->next); // Ersatt med ratt returvarde
    }
}
