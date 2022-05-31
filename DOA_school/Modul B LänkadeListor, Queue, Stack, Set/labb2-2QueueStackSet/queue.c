#include "queue.h"
#include<assert.h>

/*Har ar det viktigt att ateranvanda koden som du redan skrivit for den lankade listan.
  Ateranvandning betyder inte att man kopierar koden eller skriver av den, ateranvander
  gor man genom att anropa redan implementerade funktioner.*/

/*Funktionen ar fardig*/
Queue initializeQueue(void)
{
    return NULL;
}

int queueIsEmpty(const Queue queue)
{
    if (queue == NULL)
    {
        return 1;
    }
    return 0;	// Ersatt med ratt returvarde
}

/*Postcondition: data ar tillagt sist i kon*/
void enqueue(Queue* queue, const Data data)
{
    addFirst(queue, data);
   //Tips: Att lagga till i en ko implementerad som en lankad lista ar precis som att lagga till forst eller sist i en lankad lista
    assert(getFirstElement((*queue)) == data);
}

/* Precondition: kon Šr ej tom */
void dequeue(Queue* queue)
{
    assert(!isEmpty(*queue));
    if (*queue != NULL)
    {
        removeLast(queue);
    }
    else
    {
        printf("Queue is empty\n");
        return;
    }
    //Tips: Att ta bort fran en ko implementerad som en lankad lista ar precis som att ta bort sist eller forst i en lankad lista.
}

/* Precondition: kon Šr ej tom */
Data peekQueue(const Queue queue)
{
    assert(!isEmpty(queue));
    if (queue != NULL)
    {
        return getLastElement(queue);
    }
    else
    {
        printf("Queue is empty\n");
        return NULL;
    }
}


/* Anvands for testning och felsokning
   Tips: det kan vara bra att ocksa notera i utskriften vart borjan respektive slutet pŒ kon Šr */
void printQueue(const Queue queue, FILE *textfile)
{
    printf("back-->  ");
    printList(queue, textfile);
    printf("  <--front\n\n");
}


