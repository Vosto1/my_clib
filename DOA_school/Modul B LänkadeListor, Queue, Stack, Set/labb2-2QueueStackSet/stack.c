#include "stack.h"
#include<assert.h>

/*Funktionen ar fardig*/
Stack initializeStack(void)
{
    return createEmptyList();
}

int stackIsEmpty(const Stack stack)
{
    return isEmpty(stack);
}

/* Postcondition 'data' ligger overst pŒ stacken */
void push(Stack* stack, const Data data)
{
    addFirst(stack, data);
    assert(getFirstElement(*stack)==data);
}

/* Precondition: stacken far inte vara tom */
void pop(Stack* stack)
{
    assert(!isEmpty(*stack));
    removeFirst(stack);
}

/* Precondition: stacken far inte vara tom */
Data peekStack(const Stack stack)
{
    assert(!isEmpty(stack));
    if (stack != NULL)
    {
        return getFirstElement(stack);
    }
    else
    {
        printf("Stack is empty\n");
        return NULL;
    }
}

/* Anvands for testning och felsokning
 Tips: det kan vara bra att ocksa notera i urskriften vart toppen pa stacken ar */
void printStack(const Stack stack, FILE *textFile)
{
    printf("top-->  ");
    printList(stack, textFile);
    printf("  <-- bottom\n\n");
}
