#include "list.h"
#include <stdlib.h>
#include <assert.h>

/*Det Šr helt tillŒtet att lŠgga till egna hjŠlpfunktioner men de befintliga funktionerna fŒr inte Šndras*/

/*Hjalpfunktion till add
  Allokerar minne for en ny nod
  om allokeringen lyckades initieras data samt pekare (pekare initieras till NULL).
  Den nya noden (eller NULL) returneras.*/
static struct node* createListNode(const Data data)
{
    //List är en node ptr
    List tempNodeptr = (List)malloc(sizeof(struct node));
    if (tempNodeptr != NULL)
    {
        tempNodeptr->data = data;
        tempNodeptr->next = NULL;
        return tempNodeptr;
    }
    //Glom inte att testa sa att allokeringen lyckades innan du initierar noden
    else
    {
        printf("Error allocating memory.\n");
        return NULL; //Ersatt med ratt returvarde
    }
}

/*Returnera en tom lista - funktionen ar fardig*/
List createEmptyList(void)
{
    return NULL;
}


/*Ar listan tom?
  Returnerar 1 om den Šr tom, annars 0*/
int isEmpty(const List list)
{
    if (list == NULL)
        return 1;
    return 0; //ersatt med ratt returvarde
}

/*Lagg till nod forst i listan*/
/*Postcondition: Det nya datat ligger forst i listan (testa med assert)*/
void addFirst(List *list, const Data data)
{
    List temp = createListNode(data);
    if (temp != NULL)
    {
        if (*list == NULL)
        {
            *list = temp;
            assert(getFirstElement(*list) == data);
        }
        else
        {
            List tempPtr = *list;
            *list = temp;
            (*list)->next = tempPtr;
            assert(getFirstElement(*list) == data);
        }

    }
    else
    {
        return;
    }
    //Anropa createListNode for att skapa den nya noden
    //Glom inte att testa att den nya noden faktiskt kunde skapas/tilldelas minne innan du fortsatter
    //Tank pa att listan kan vara tom nar en ny nod laggs till
}

/*Lagg till nod sist i listan
  Tips, nar du hittat ratt plats kan du anvanda funktionen addFirst for att lagga till*/
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

/*Ta bort forsta noden i listan
  Precondition: listan ar inte tom (testa med assert)
  Noden ska lankas ur och minnet frigoras, resten av listan ska finnas kvar*/
void removeFirst(List *list)
{
    assert(!isEmpty(*list)); //kommentera för att menyn ska fungera
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
   

    //Glom inte att frigora minnet for den nod som lankas ur listan.
    //Tank pa att listans huvud efter bortlankningen maste peka pa den nod som nu ar forst.
}

/*Ta bort sista noden i listan
  Precondition: listan ar inte tom (testa med assert)t*/
void removeLast(List *list)
{
    assert(!isEmpty(*list));
    if ((*list)->next == NULL)
    {
        free((*list));
        *list = NULL;
    }
    else if ((*list)->next->next == NULL)
    {
        free((*list)->next);
        (*list)->next = NULL;
    }
    else
    {
        removeLast(&(*list)->next);
    }
    //Glom inte att frigora minnet for den nod som lankas ur listan.
    //Tank pa att den nod som nu ar sist inte pekar nagonstans, dess pekare maste nollstallas
}

/*Ta bort data ur listan (forsta forekomsten)
  Returnera 1 om datat finns, annars 0
  Tips, nar du hittar ratt nod kan du anvanda en av de ovanstaende funktionerna for att ta bort noden*/
int removeElement(List *list, const Data data)
{
    if ((*list) != NULL)
    {
        if ((*list)->next == NULL)
        {
            if ((*list)->data == data)
            {
                free((*list));
                (*list) = NULL;
                return 1;
            }
            return 0;
        }
        if ((*list)->next->data == data)
        {
            List temp = (*list)->next;
            (*list)->next = (*list)->next->next;
            free(temp);
            return 1;
        }
        else if ((*list)->data == data)
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

/*Finns data i listan?
  Om datat finns returneras 1, annars 0
  Tank pa att listan kan vara tom*/
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
        search(list->next, data);
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
        return 1 + numberOfNodesInList(list->next); //Ersatt med ratt returvarde
    }
}

/*Ta bort alla noder ur listan
  Glom inte att frigora minnet
  Postcondition: Listan ar tom, *list Šr NULL (testa med assert)*/
void clearList(List *list)
{
    if ((*list) == NULL)
    {
        free((*list));
        (*list) = NULL;
        return;
    }
    clearList(&(*list)->next);
    free((*list));
    (*list) = NULL;
    assert(*list == NULL);
    //Alla noder maste tas avallokeras en och en, det racker inte att endast frigora list.
}

/*Skriv ut listan
  Vid anropet kan man ange stdout som argument 2 for att skriva ut pŒ skarmen.
  Anvanda fprintf for att skriva ut.
  Den har typen av utskriftfunktion blir mer generell da man kan valja att skriva ut till skarmen eller till fil.*/
void printList(const List list, FILE *textfile)
{
    if (list != NULL)
    {
        if (list->next == NULL)
        {
            printf("%d", list->data);
            return;
        }
        printf("%d", list->data);
        printList(list->next, textfile);
    }
}

/*Returnera forsta datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
Data getFirstElement(const List list)
{
    assert(!isEmpty(list)); //kommentera för att menyn ska fungera
    if (list != NULL)
        return list->data; //Ersatt med ratt returvarde
    else
    {
        printf("List is empty\n");
        return NULL;
    }
}

/*Returnera sista datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
Data getLastElement(const List list)
{
    assert(!isEmpty(list)); // kommenterad för att menyn ska fungera
    if (list != NULL)
    {
        if (list->next == NULL)
        {
            return list->data;
        }
        return getLastElement(list->next); //Ersatt med ratt returvarde
    }
    else
    {
        printf("List is empty.\n");
        return NULL;
    }
}
