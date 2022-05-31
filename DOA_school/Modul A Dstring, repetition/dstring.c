#include "dstring.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>


DString dstring_initialize(const char* str)
{
	// Precondition: str ar ej NULL - testas med en assert

	/* Tips:
	   - Allokera (t.ex. med malloc) tillrackligt med minne for att fa plats med str,
         glom inte att det maste finnas plats f�r \0 (nolltermineringen).
         Glom inte att testa sa att allokeringen lyckades
	   - Det �r tillatet att anv�nda inbyggda strangfunktioner (som t.ex strcpy())
	   - Kom ihag att anvanda assert() */

	// Postcondition: returvardet innehaller samma strang som 'str' - beh�ver inte testas med assert

	return NULL; // Ersatt denna rad. Den ar just nu med for att programmet ska kompilera
}

int dstring_concatenate(DString* destination, DString source)
{
    // Precondition: destination �r ej NULL
	// Precondition: *destination ar ej NULL
	// Precondition: source ar ej NULL
    // Preconditions testas med asserts.

	/* Tips: 
	   - Tank p� att *destination redan ar en dynamiskt allokerad strang
	   - Tank p� att vi kommer att behova allokera mer minne for att fa plats med den har langre strangen.
         Minnet som *destination pekar pa behover utokas (realloc), hur stort ska det "nya" minnet bli?
         Glom inte att nolltermineringen maste fa plats.
         Nar du allokerar minne dynamiskt maste du alltid testa sa att allokeringen lyckades innan du
         borjar jobba med minnet.
       - Det ar tillatet att anvanda inbyggda funktioner (som t.ex. strcat()).
    */

	// Postcondition: *destination innehaller den gamla strangen ihopslagen med source - beh�ver inte testas med assert.
	return -1; // Ersatt denna rad. Den ar just nu med for att programmet ska kompilera, vad ar det som ska returneras?.
}

void dstring_truncate(DString* destination, unsigned int truncatedLength)
{
	// Precondition: destination �r ej NULL, *destination ar ej NULL
    // l�ngden (truncateLength) f�r inte vara negativ
    // Preconditions testas med asserts

	/* Tips:
	   - Anvand realloc for att frigora det overflodiga minnet
         (realloc frigor forst minnet och allokerar sedan nytt for den angivna storleken)
	   - glom inte nolltermineringen
     
     Vad h�nder om truncateLength �r l�ngre �n vad str�ngen �r?*/

	// Postcondition: *destination ar inte langre an 'truncatedLength' tecken - beh�ver inte testas med assert
}

void dstring_print(DString str, FILE* textfile)
{
	// Precondition: textfile ar inte NULL - maste testas innan du forsoker skriva till filen
    /*Tank p� att filpekaren kopplas till filen innan funktionen anropas*/

	/* filpekaren far inte stangas, filen ska fungera som vanligt efter anropet */
    /*Den har funktionen ska vara generell, du ska kunna anvanda den for att skriva ut till
     en fil eller for att skriva ut i konsollen (pa skarmen). Om den anvands for att skriva till
     en extern fil sa kan du anta att filen oppnas och stangs innan respektive efter anrop till
     funktionen. Om den anvands for att skriva ut pa skarmen kan du anta att du far stdout till den
     andra parametern.
     
     Du ska inte skriva olika kod for att skriva till fil och till konsoll. Samma kod ska fungera for bada.*/
}

void dstring_delete(DString* stringToDelete)
{
	// Precondition: stringToDelete ar inte NULL

	// Postcondition: *stringToDelete ar NULL och minnet ar frigjort - beh�ver inte testas med assert
}
