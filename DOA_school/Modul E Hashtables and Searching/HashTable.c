#define _CRT_SECURE_NO_WARNINGS // Behovs for vissa funktioner i visual studio
#include "HashTable.h"
#include "Bucket.h"
#include<assert.h>
#include<stdlib.h>
#include<stdio.h>

	// Anvands for att markera en ledig plats i Hashtabellen


/* Denna funktion tar en nyckel och returnerar ett hash-index
dvs ett index till arrayen som är Hashtabellen */
static int hash(Key key, int tablesize)
{
    return key % tablesize;
}

/*Leta framåt enligt principen öppen adressering
 Antalet krockar returneras via pekaren col i parameterlistan*/
static int linearProbe(const HashTable* htable, Key key, unsigned int *col)
{
    unsigned int size = getSize(htable);
    int index = 0;
    unsigned int i = 0;
    while (i < (*htable).size)
    {
        index = hash(key + i, size);
        if ((*htable).table[index].key == UNUSED || (*htable).table[index].key == key)
        {
            (*htable).table[index].key = key;
            *col = i;
            return index;
        }
        i += 1;
    }
    printf("Hashtable overflow!\n");
    *col = i;
    return -1;
}

/*Allokera minne för hashtabellen*/
HashTable createHashTable(unsigned int size)
{
    // Dessa tva rader ar bara till for att labskelettet ska kompilera. Ta bort dessa nar du skriver funktionen.
    HashTable temp;
    temp.table = (struct Bucket*)malloc(sizeof(struct Bucket) * size);
    if (temp.table != NULL)
    {
        for (unsigned int i = 0; i < size; i++)
        {
            temp.table[i].key = UNUSED;
        }
        temp.size = size;
        return temp;
    }
    else
    {
        printf("Allocation error.\n");
        temp.size = -1;
        return temp;
    }
}

/* Satter in paret {key,data} i Hashtabellen, om en nyckel redan finns ska vardet uppdateras */
/* Returnerar antalet krockar (som räknas i linearProbe() )*/
unsigned int insertElement(HashTable* htable, const Key key, const Value value)
{
    int colisions = 0;
    int index = linearProbe(htable, key, &colisions);
    if (key == (*htable).table[index].key)
    {
        (*htable).table[index].value = value;
        assert(lookup(htable, key) != NULL);
    }
    else
        printf("Error.\n");
	// Postcondition: det finns ett element for key i tabellen (anvand lookup() for att verifiera)
    return colisions; //Ersatt med ratt varde
}

static unsigned int findIndex(HashTable* htable, unsigned int valueToSearchFor)
{
    unsigned int size = getSize(htable);
    int index = 0;
    unsigned int i = 0;
    while (i < (*htable).size)
    {
        index = hash(valueToSearchFor + i, size);
        if ((*htable).table[index].key == valueToSearchFor)
        {
            return index;
        }
        i += 1;
    }
}

/* Tar bort datat med nyckel "key" */
void deleteElement(HashTable* htable, const Key key)
{
    unsigned int size = getSize(htable);
    if (lookup(htable, key) != NULL)
    {
        struct Bucket tempNode;
        unsigned int index = findIndex(htable, key);
        (*htable).table[index].key = UNUSED;
        unsigned int j = 0;
        for (unsigned int i = 1; i < (*htable).size && (*htable).table[(index + i) % htable->size].key != UNUSED; i++)
        {
            j = hash(index + i, size);
            tempNode.key = (*htable).table[j].key;
            tempNode.value = (*htable).table[j].value;
            (*htable).table[j].key = UNUSED;
            insertElement(htable, tempNode.key, tempNode.value);
        }
    }
    assert(lookup(htable, key) == NULL); // Postcondition: inget element med key finns i tabellen (anvand lookup() for att verifiera)
}

/* Returnerar en pekare till vardet som key ar associerat med eller NULL om ingen sadan nyckel finns */
const Value* lookup(const HashTable* htable, const Key key)
{
    int size = getSize(htable);
    int index = 0;
    unsigned int i = 0;
    while (i < (*htable).size)
    {
        index = hash(key + i, size);
        if ((*htable).table[index].key == key)
        {
            return &(*htable).table[index].value;
        }
        else if ((*htable).table[index].key == 0)
            break;
        i += 1;
    }
    return NULL;
}

/* Tommer Hashtabellen */
void freeHashTable(HashTable* htable)
{
    free((*htable).table);
    (*htable).table = NULL;
    (*htable).size = 0;
    assert(getSize(htable) == 0);
	// Postcondition: hashtabellen har storlek 0
}

/* Ger storleken av Hashtabellen */
unsigned int getSize(const HashTable* htable)
{
    return (*htable).size; // Ersatt med ratt varde
}

/* Denna for att ni enkelt ska kunna visualisera en Hashtabell */
void printHashTable(const HashTable* htable)
{
    printf("\n");
    printf("Print table:\n(Only prints tables with values assigned)\n\n");
    for (unsigned int i = 0; i < (*htable).size; i++)
    {
        if ((*htable).table[i].key != -1)
        {
            printf("index: %d\t", i);
            printf("key: %d\n", (*htable).table[i].key);
            printf("Name: %s\n", (*htable).table[i].value.name);
            printf("pNr: %d\n", (*htable).table[i].value.personalNumber);
            printf("Weight: %.2f\n", (*htable).table[i].value.weight);
            printf("\n\n");
        }
    }
    printf("total table size: %d", (*htable).size);
    printf("\n");
	// Tips: anvand printPerson() i Person.h for att skriva ut en person
}
