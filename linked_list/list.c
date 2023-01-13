#include "list.h"
#include <stdlib.h>
#include <assert.h>


/*doubly-linked list*/
typedef struct node
{
    const void* data;
    struct node *next;
    struct node *previous;
}node;

struct head {
    node* first;
    node* last;
};
/*Hjalpfunktion till add
  Allokerar minne for en ny nod
  om allokeringen lyckades initieras data samt pekare (pekare initieras till NULL).
  Den nya noden (eller NULL) returneras.*/
static struct node *create_list_node(const void* data)
{
    // ll �r en node ptr
    node* tempNodeptr = (node*)malloc(sizeof(struct node));
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
bool ll_is_empty(const ll* head)
{
    return (*head)->first == NULL;
}

void static _add_first(node** list, const void* data)
{
    // skapa ny nod
    node* temp = create_list_node(data);
    if (temp != NULL)
    {
        if (*list == NULL)
            *list = temp;
        else
        {
            node* tempPtr = *list;
            *list = temp;
            (*list)->next = tempPtr;
            tempPtr->previous = *list;
        }
    }
    else
        return;
    assert(!ll_is_empty(*list));
}

/*Lagg till nod forst i listan*/
void ll_add_first(ll *head, const void* data)
{
    _add_first(&(*head)->first, data);
}

static void _add_last(node** list, const void* data, ll* head)
{
    if ((*list)->next == NULL)
    {
        node* temp = create_list_node(data);
        if (temp != NULL)
        {
            (*list)->next = temp;
            (*list)->next->previous = *list;
            (*head)->last = temp;
        }
    }
    else
        _add_last(&(*list)->next, data, head);
}

/*Lagg till nod sist i listan*/
void ll_add_last(ll *head, const void* data)
{
    if ((*head)->first == NULL)
    {
        node* temp = create_list_node(data);
        if (temp != NULL)
        {
            (*head)->first = temp;
            (*head)->last = (*head)->first;
        }
    }
    else
    {
        _add_last(&(*head)->first, data, head);
    }
}


static const void* _remove_first(node** list, ll* head)
{
    node* temp = (*list);
    (*list) = (*list)->next;
    if ((*list) != NULL)
        (*list)->previous = NULL;
    else
        (*head)->last = NULL;
    const void* element = temp->data;
    free(temp);
    return element;
}
/*Ta bort forsta noden i listan*/
const void* ll_remove_first(ll *head)
{
    assert(!ll_is_empty(*head)); // kommentera f�r att menyn ska fungera
    return _remove_first(&(*head)->first, head);
}

/*Ta bort sista noden i listan*/
const void* ll_remove_last(ll *head)
{
    assert(!ll_is_empty(*head));
    node* rm = (*head)->last;
    const void* element = rm->data;
    (*head)->last = rm->previous;
    (*head)->last->next = NULL;
    free(rm);
    return element;
}

static const void* _remove_element(node** list, const void* data, int (*compare)(const void*, const void*))
{
    if ((*list) != NULL)
    {
        if ((*compare)((*list)->data, (const void*)data))
        {
            node* temp = (*list);
            (*list) = (*list)->next;
            if ((*list) != NULL)
                (*list)->previous = temp->previous;
            const void* element = temp->data;
            free(temp);
            return element;
        }
        else
            return _remove_element(&(*list)->next, data, compare);
    }
    else
        return NULL;
}
/*Ta bort data ur listan (forsta forekomsten)
  Returnera true om datat finns, annars false*/
const void* ll_remove_element(ll *head, const void* data, int (*compare)(const void*, const void*))
{
    return _remove_element(&(*head)->first, data, compare);
}

static const void* _search(node** list, const void* data, int (*compare)(const void*, const void*))
{
    if ((*list) != NULL)
    {
        if ((*compare)((*list)->data, (const void*)data))
            return (*list)->data;
        return _search(&(*list)->next, data, compare);
    }
    else
        return NULL;
}
/*ll_search for data in the list
Om datat finns returneras true, annars false*/
const void* ll_search(const ll* head, const void* data, int (*compare)(const void*, const void*))
{
    return _search(&(*head)->first, data, compare);
}


static uint _node_count(node** list)
{
    if (list == NULL)
        return 0;
    else
        return 1 + _node_count(&(*list)->next);
}
/*Returnera antalet noder i listan*/
uint ll_node_count(const ll* head)
{
    return _node_count(&(*head)->first);
}


static void _clear(node** list, void (*freeObject)(void*))
{
    if ((*list)->next == NULL)
        return;
    else
        _clear(&(*list)->next, freeObject);
    (*freeObject)((void*)(*list)->data);
    free((*list));
    (*list) = NULL;
    assert(*list == NULL);
}
/*Ta bort alla noder ur listan
  Glom inte att frigora minnet
  Postcondition: Listan ar tom, *list �r NULL (testa med assert)*/
void ll_clear(ll *head, void (*freeObject)(void*))
{
    _clear(&(*head)->first, freeObject);
}

/*Returnera forsta datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
const void* ll_get_first(const ll* head)
{
    assert(!ll_is_empty(head));
    return (*head)->first->data;
}

/*Returnera sista datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
const void* ll_get_last(const ll* head)
{
    assert(!ll_is_empty(head)); // kommentera f�r att menyn ska fungera
    return (*head)->last->data;
}
