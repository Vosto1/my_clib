#include <stdio.h>
#include "list-1.h" // EXAMINATION, la på -1 i alla c och h filer som behövde det istället för att byta namn på alla filer.
#include <assert.h>

void menu(List head);
void testFunctionExamination(List head);

int main(void)
{
    List head = createEmptyList();
    
    testFunctionExamination(head);
    //menu(head);
    
    return 0;
}

void menu(List head)
{
    int choice;
    Data data;
    char c; //Anvands endast for att rensa lasbufferten
    
    do
    {
        printf("\n\n--------------MENU--------------\n"
               "1 - Print list\n"
               "2 - Add data first in list\n"
               "3 - Add data last in list\n"
               "4 - Remove data from list\n"
               "5 - Number of nodes in list\n"
               "6 - Search in list\n"
               "7 - Clear list (removes all nodes)\n"
               "8 - End program\n"
               "-----------------------------------\n"
               "Choice: ");
        
        scanf("%d", &choice);
        while((c = getchar()) != '\n' && c != EOF); //Rensar lasbufferten
        
        switch(choice)
        {
            case 1: printf("List: ");
                    printList(head, stdout); break;
                
            case 2: printf("Data to add first: ");
                    scanf("%d%*c", &data);
                    addFirst(&head, data);
                    printf("List: ");
                    printList(head, stdout);
                    break;
                
            case 3: printf("Data to add last: ");
                    scanf("%d%*c", &data);
                    addLast(&head, data);
                    printf("List: ");
                    printList(head, stdout);
                    break;
                
            case 4: printf("Data to remove: ");
                    scanf("%d%*c", &data);
                    if(removeElement(&head, data) == 1)
                        printf("One occurance of node with data %d is removed\n", data);
                    else
                        printf("No node with data %d\n", data);
                    printf("List: ");
                    printList(head, stdout);
                    break;
                
            case 5: printf("Number of nodes in list: %d\n", numberOfNodesInList(head));
                    break;
                
            case 6: printf("Data to search for: ");
                    scanf("%d", &data);
                    int occurances = search(head, data);
                    if(occurances > 0)
                    {
                        printf("%d found %d times in list\n", data, occurances);
                        printList(head, stdout);
                    }
                    else
                    {
                        printf("%d not found in list\n", data);
                        printList(head, stdout);
                    }
                    break;
                
            case 7: clearList(&head); break;
                
            case 8: printf("\nEnding program");
                
            default: printf("\nWrong input");
        }
    }while(choice != 8);
}


void testFunctionExamination(List head)
{
    printf("Starting test\n\n");
    assert(isEmpty(head));
    addFirst(&head, 6);
    addFirst(&head, 5);
    addFirst(&head, 6);
    addFirst(&head, 3);
    addFirst(&head, 5);
    addFirst(&head, 5);
    assert(numberOfNodesInList(head) == 6);
    assert(getFirstElement(head) == 3);
    assert(getLastElement(head) == 6);
    printList(head, stdout); //kontrollera manuellt
    //listan består nu av 3(1st)-5(3st)-6(2st)
    printf("\n");
    addLast(&head, 7);
    addLast(&head, 8);
    addLast(&head, 8);
    addLast(&head, 5);
    assert(numberOfNodesInList(head) == 10);
    assert(getFirstElement(head) == 3);
    assert(getLastElement(head) == 8);
    assert(search(head, 8) == 2);
    assert(search(head, 5) == 4);
    assert(search(head, 7) == 1);
    printList(head, stdout); //kontrollera manuellt
    //listan består nu av 3(1st)-5(4st)-6(2st)-7(1st)-8(2st)
    printf("\n");
    removeElement(&head, 5);
    assert(numberOfNodesInList(head) == 9);
    assert(getFirstElement(head) == 3);
    assert(getLastElement(head) == 8);
    assert(search(head, 5) == 3);
    printList(head, stdout); //kontrollera manuellt
    //listan består nu av 3(1st)-5(3st)-6(2st)-7(1st)-8(2st)
    printf("\n");
    removeElement(&head, 8);
    assert(numberOfNodesInList(head) == 8);
    assert(getFirstElement(head) == 3);
    assert(getLastElement(head) == 8);
    printList(head, stdout); //kontrollera manuellt
    //listan består nu av 3(1st)-5(3st)-6(2st)-7(1st)-8(1st)
    printf("\n");
    removeElement(&head, 3);
    assert(numberOfNodesInList(head) == 7);
    assert(getFirstElement(head) == 5);
    assert(getLastElement(head) == 8);
    assert(search(head, 3) == 0);
    printList(head, stdout); //kontrollera manuellt
    //listan består nu av 5(3st)-6(2st)-7(1st)-8(1st)
    printf("\n");
    removeElement(&head, 6);
    removeElement(&head, 6);
    assert(numberOfNodesInList(head) == 5);
    assert(getFirstElement(head) == 5);
    assert(getLastElement(head) == 8);
    printList(head, stdout); //kontrollera manuellt
    //listan består nu av 5(3st)-7(1st)-8(1st)
    printf("\n");
    assert(removeElement(&head, 6) == 0);
    assert(numberOfNodesInList(head) == 5);
    printList(head, stdout); //kontrollera manuellt
    //listan består fortfarande av 5(3st)-7(1st)-8(1st)
    printf("\n");
    addFirst(&head, 7);
    addLast(&head, 9);
    assert(numberOfNodesInList(head) == 7);
    assert(getFirstElement(head) == 5);
    assert(getLastElement(head) == 9);
    printList(head, stdout); //kontrollera manuellt
    //listan består nu av 5(3st)-7(2st)-8(1st)-9(1st)
    printf("\n");
    removeElement(&head, 5);
    removeElement(&head, 7);
    removeElement(&head, 7);
    removeElement(&head, 5);
    removeElement(&head, 8);
    removeElement(&head, 5);
    removeElement(&head, 9);
    assert(isEmpty(head)); //Listan ska nu vara tom
    addFirst(&head, 7);
    addLast(&head, 9);
    addFirst(&head, 7);
    addFirst(&head, 3);
    assert(numberOfNodesInList(head) == 4);
    assert(getFirstElement(head) == 3);
    assert(getLastElement(head) == 9);
    printList(head, stdout); //kontrollera manuellt
    //listan består nu av 3(1st)-7(2st)-9(1st)
    printf("\n");
    clearList(&head);
    assert(isEmpty(head));
    assert(search(head, 5) == 0);//Testar att soka i en tom lista
    assert(removeElement(&head, 3) == 0); //testar att ta bort från tom lista
    assert(numberOfNodesInList(head) == 0); //testar att räkna noderna i en tom lista
    
    printf("\n\nCongratulations! Your program passed the test\n");
}

