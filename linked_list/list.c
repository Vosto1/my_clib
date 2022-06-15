#include "list.h"
#include <stdlib.h>
#include <assert.h>


/*Hjalpfunktion till add
  Allokerar minne for en ny nod
  om allokeringen lyckades initieras data samt pekare (pekare initieras till NULL).
  Den nya noden (eller NULL) returneras.*/
static struct node* createListNode(const voidp data)
{
    //ll �r en node ptr
    ll tempNodeptr = (ll)malloc(sizeof(struct node));
    if (tempNodeptr != NULL)
    {
        tempNodeptr->data = data;
        tempNodeptr->next = NULL;
        tempNodeptr->previous = NULL;
        return tempNodeptr;
    }
    else
        return NULL;
}

/*Returnera en tom lista*/
ll ll_create_empty(void)
{
    return NULL;
}


/*Ar listan tom?
  Returnerar true om den �r tom, annars false*/
bool ll_is_empty(const ll list)
{
    if (list == NULL)
        return true;
    return false;
}

/*Lagg till nod forst i listan*/
void ll_add_first(ll* list, const voidp data)
{
    //skapa ny nod
    ll temp = createListNode(data);
    if (temp != NULL)
    {
        if (*list == NULL)
            *list = temp;
        else
        {
            ll tempPtr = *list;
            *list = temp;
            (*list)->next = tempPtr;
            tempPtr->previous = *list;
        }

    }
    else
        return;
    assert(!ll_is_empty(*list));
}

/*Lagg till nod sist i listan*/
void ll_add_last(ll* list, const voidp data)
{
    if ((*list) == NULL)
        ll_add_first(list, data);
    else if ((*list)->next == NULL)
    {
        ll temp = createListNode(data);
        if (temp != NULL)
        {
            (*list)->next = temp;
            (*list)->next->previous = *list;
        }
    }
    else
        ll_add_last(&(*list)->next, data);
}

/*Ta bort forsta noden i listan*/
void ll_remove_first(ll* list)
{
    assert(!ll_is_empty(*list)); //kommentera f�r att menyn ska fungera
    if (*list != NULL)
    {
        ll temp = (*list);
        (*list) = (*list)->next;
        if((*list) != NULL)
            (*list)->previous = NULL;
        free(temp);
    }
}

/*Ta bort sista noden i listan*/
void ll_remove_last(ll* list)
{
    assert(!ll_is_empty(*list));
    if ((*list)->next == NULL)
    {
        if ((*list)->previous != NULL)
        {
            ll temp = (*list)->previous;
            temp->next = NULL;
        }
        free((*list));
        *list = NULL;
    }
    else
        ll_remove_last(&(*list)->next);
}

/*Ta bort data ur listan (forsta forekomsten)
  Returnera true om datat finns, annars false*/
bool ll_remove_element(ll* list, const voidp data, int (*compare)(cvoidp,cvoidp))
{
    if ((*list) != NULL)
    {
        if ((*compare)((*list)->data, (cvoidp)data))
        {
            ll temp = (*list);
            (*list) = (*list)->next;
            if((*list) != NULL)
                (*list)->previous = temp->previous;
            free(temp);
            return true;
        }
        else
            return ll_remove_element(&(*list)->next, data, compare);
    }
    else
        return false;
}

/*ll_search for data in the list
Om datat finns returneras true, annars false*/
bool ll_search(const ll list, const voidp data, int (*compare)(cvoidp,cvoidp))
{
    if (list != NULL)
    {
        if ((*compare)(list->data, (cvoidp)data))
            return true;
        ll_search(list->next, data, compare);
    }
    else
        return false;
}

/*Returnera antalet noder i listan*/
int ll_node_count(const ll list)
{
    if (list == NULL)
        return 0;
    else
        return 1 + ll_node_count(list->next);
}

/*Ta bort alla noder ur listan
  Glom inte att frigora minnet
  Postcondition: Listan ar tom, *list �r NULL (testa med assert)*/
void ll_clear(ll* list)
{
    if((*list)->next == NULL)
        return;
    else
        ll_clear(&(*list)->next);
    free((*list));
    (*list) = NULL;
    assert(*list == NULL);
}

/*Returnera forsta datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
voidp ll_get_first(const ll list)
{
    return list->data;
}

/*Returnera sista datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
voidp ll_get_last(const ll list)
{
    assert(!ll_is_empty(list)); //kommentera f�r att menyn ska fungera
    if (list != NULL)
    {
        if (list->next == NULL)
            return list->data;
        return ll_get_last(list->next); //Ersatt med ratt returvarde
    }
}
