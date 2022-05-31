#include "set.h"
#include <assert.h>

/*Funktionen ar fardig*/
Set initializeSet(void)
{
	return createEmptyList();
}

/* Tips: tank pa att inte lagga till elementet om det redan finns i *set
   Postcondition: 'element' finns i set (tips: anvand isInSet() for att verifiera) */
void addToSet(Set* set, const Data element)
{
    if (!(search(*set, element)))
    {
        addFirst(set, element);
        assert(getFirstElement(*set) == element);
    }
    else
    {
        printf("Data already exist in set.\n");
        return;
    }
}

/* Postcondition: 'element' finns INTE i set (tips: anvand isInSet() for att verifiera) */
void removeFromSet(Set* set, const Data element)
{
    int err = removeElement(set, element);
    assert(!(search(*set, element)));
    if (err == 1)
    {
        printf("Element removed\n");
        return;
    }
    else
    {
        printf("Element doesnt exist in set\n");
        return;
    }
}


int isInSet(const Set set, const Data element)
{
    return search(set, element); // Ersatt med ratt returvarde
}

/* Anvands for testning och felsokning */
void printSet(const Set set, FILE *textfile)
{
    printf("\n");
    printList(set, textfile);
    printf("\n\n");
}

