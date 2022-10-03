#ifndef DSTRING_H
#define DSTRING_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "../datatype.h"

typedef char *dstring;

/* Returnerar en strang som innehaller samma text som textstrangen 's'.
  Den returnerade strangen ar dynamiskt allokerad (dvs, p� heapen) */
dstring ds_init(const char *s);

/* Slar ihop originalstrangen med src
  Returnera 0 om det inte gick att allokera tillrackligt med minne
  f�r att sla ihop strangarna, annars 1 */
int ds_concat(dstring *dest, dstring src);

/* Forkortar *dest sa att den innehaller hogst truncated_len tecken
  Om 'truncated_len' ar langre an strangens langd hander ingenting */
void ds_truncate(dstring *dest, unsigned int truncated_len);

/* Skriver strangen till en textfil.
   Textfilen forvantas redan vara oppnad och fortsatter vara oppnad efter anropet */
/*OBS - det racker att du testar att denna funktion skriver ut i konsollen/skarmen
 Se kommentarer i funktionsdefinitionen for mer information*/
void ds_print(dstring s, FILE *file);

/* Frigor minnet f�r en dynamisk strang och satter strangen (*del) till NULL */
void ds_delete(dstring *del);

//TODO:
// Fix this implementation
// create better tests
//functions to make:
// string.split
// string.replace

#endif
