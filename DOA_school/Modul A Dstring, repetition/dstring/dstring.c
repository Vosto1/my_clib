#include "dstring.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>


DString dstring_initialize(const char* str)
{
	// Precondition: str ar ej NULL - testas med en assert
	assert(str != NULL);
	/* Tips:
	   - Allokera (t.ex. med malloc) tillrackligt med minne for att fa plats med str,
         glom inte att det maste finnas plats för \0 (nolltermineringen).
         Glom inte att testa sa att allokeringen lyckades
	   - Det är tillatet att använda inbyggda strangfunktioner (som t.ex strcpy())
	   - Kom ihag att anvanda assert() */
	const char* temp;
	temp = (char*)malloc(sizeof(char) * (strlen(str) + 1));
	assert(temp != NULL);
	strcpy(temp, str);
	assert(strlen(temp) == strlen(str));
	// Postcondition: returvardet innehaller samma strang som 'str' - behöver inte testas med assert
	assert(!(strcmp(temp, str)));
	return temp; // Ersatt denna rad. Den ar just nu med for att programmet ska kompilera
}

int dstring_concatenate(DString* destination, DString source)
{
    // Precondition: destination är ej NULL
	// Precondition: *destination ar ej NULL
	// Precondition: source ar ej NULL
    // Preconditions testas med asserts.
	assert(destination != NULL);
	assert(*destination != NULL);
	assert(source != NULL);
	/* Tips: 
	   - Tank på att *destination redan ar en dynamiskt allokerad strang
	   - Tank på att vi kommer att behova allokera mer minne for att fa plats med den har langre strangen.
         Minnet som *destination pekar pa behover utokas (realloc), hur stort ska det "nya" minnet bli?
         Glom inte att nolltermineringen maste fa plats.
         Nar du allokerar minne dynamiskt maste du alltid testa sa att allokeringen lyckades innan du
         borjar jobba med minnet.
       - Det ar tillatet att anvanda inbyggda funktioner (som t.ex. strcat()).
    */
	int test1 = strlen(source), test2 = strlen(*destination);

	int result = 0, len = (strlen(*destination) + strlen(source) + 1);
	DString temp = (DString)realloc(*destination, sizeof(char)*len);
	assert(temp != NULL);
	if (temp != NULL)
	{
		result = 1;
		*destination = temp;
		strcat(*destination, source);
	}
	else
		return result;
	// Postcondition: *destination innehaller den gamla strangen ihopslagen med source - behöver inte testas med assert.
	return result; // Ersatt denna rad. Den ar just nu med for att programmet ska kompilera, vad ar det som ska returneras?.
}

void dstring_truncate(DString* destination, unsigned int truncatedLength)
{
	// Precondition: destination är ej NULL, *destination ar ej NULL
    // längden (truncateLength) får inte vara negativ
    // Preconditions testas med asserts
	assert(*destination != NULL);
	assert(destination != NULL);
	assert(truncatedLength > 0);

	DString temporary;
	temporary = (char*)malloc(sizeof(char) * (strlen(*destination) + 1));
	if (temporary == NULL) 
	{
		printf("Error allocating memory\n");
		return;
	}
	strcpy(temporary, *destination);
	/* Tips:
	   - Anvand realloc for att frigora det overflodiga minnet
         (realloc frigor forst minnet och allokerar sedan nytt for den angivna storleken)
	   - glom inte nolltermineringen
     
     Vad händer om truncateLength är längre än vad strängen är?*/
	
	if (truncatedLength < strlen(*destination))
	{
		*(*destination + truncatedLength) = '\0';
		DString temp = (DString)realloc(*destination, (truncatedLength + 1) * sizeof(char));
		assert(temp != NULL);
		if (temp != NULL)
		{
			*destination = temp;
			assert(strlen(*destination) == truncatedLength);
		}
		else
		{
			printf("Error allocating memory\n");
			strcpy(*destination, temporary);
			free(temporary);
			return;
		}
	}
	else
	{
		free(temporary);
		return;
	}
	// Postcondition: *destination ar inte langre an 'truncatedLength' tecken - behöver inte testas med assert
	assert(strlen(*destination) == truncatedLength);
	free(temporary);
}

void dstring_print(DString str, FILE* textfile)
{
	// Precondition: textfile ar inte NULL - maste testas innan du forsoker skriva till filen
    /*Tank på att filpekaren kopplas till filen innan funktionen anropas*/
	assert(textfile != NULL);

	/* filpekaren far inte stangas, filen ska fungera som vanligt efter anropet */
    /*Den har funktionen ska vara generell, du ska kunna anvanda den for att skriva ut till
     en fil eller for att skriva ut i konsollen (pa skarmen). Om den anvands for att skriva till
     en extern fil sa kan du anta att filen oppnas och stangs innan respektive efter anrop till
     funktionen. Om den anvands for att skriva ut pa skarmen kan du anta att du far stdout till den
     andra parametern.
     
     Du ska inte skriva olika kod for att skriva till fil och till konsoll. Samma kod ska fungera for bada.*/
	int length = strlen(str);
	for (int i = 0; i < length; i++)
	{
		fprintf(textfile, "%c", str[i]);
	}
}

void dstring_delete(DString* stringToDelete)
{
	// Precondition: stringToDelete ar inte NULL
	assert(*stringToDelete != NULL);
	free(*stringToDelete);
	*stringToDelete = NULL;
	// Postcondition: *stringToDelete ar NULL och minnet ar frigjort - behöver inte testas med assert
}
