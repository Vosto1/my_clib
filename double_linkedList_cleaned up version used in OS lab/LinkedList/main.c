#include <string.h>
#include "list.h"

int main(void) {
    List head = createEmptyList();
    //testFunction(head);

    planet_type pl1;
    pl1.life = 0;
    strcpy(pl1.name, "testpl1");
    planet_type pl2;
    pl2.life = 1;
    strcpy(pl2.name, "testpl2");
    planet_type pl3;
    pl3.life = -1;
    strcpy(pl3.name, "testpl3");
    planet_type pl4;
    pl4.life = 1;
    strcpy(pl4.name, "testpl4");
    addFirst(&head, pl1);
    addFirst(&head, pl2);
    addFirst(&head, pl3);
    addFirst(&head, pl4);
    removeDead(&head);
    return 0;
}